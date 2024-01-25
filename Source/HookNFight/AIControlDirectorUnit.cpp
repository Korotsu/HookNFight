// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControlDirectorUnit.h"

#include "AIRemoteControlPlugin.h"
#include "CE_MeleeEnemy.h"
#include "CE_ArcherEnemy.h"
#include "A_EnemySpawner.h"
#include "SMC_GlowyRotatingInclinedCube.h"
#include "SC_AIcDuWayPoint.h"
#include "BPaCpp_FunctionLibrary.h"
#include "I_DebugMesssagesManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AAIControlDirectorUnit::AAIControlDirectorUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereColision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
	SetRootComponent(SphereColision);

	MainSphere = CreateDefaultSubobject<USMC_GlowySphere>(TEXT("MainControlSphere"));
	MainSphere->SetupAttachment(RootComponent);
	MainSphere->SetRelativeLocation({0, 0, 0});


	static ConstructorHelpers::FObjectFinder<UParticleSystem> baseLightningParticleSystemAsset(TEXT("/Game/j_cellier/Ressources_Julie/Particule/Eclair/EclairP"));
	if (baseLightningParticleSystemAsset.Object) LightningParticleSystem = baseLightningParticleSystemAsset.Object;
}

void AAIControlDirectorUnit::CreateNewFragments(int NumberOfFragments)
{
	for (int i = 0; i < NumberOfFragments; i++)
	{
		USceneComponent* NewAttach = NewObject<USceneComponent>(this);
		NewAttach->SetupAttachment(RootComponent);
		NewAttach->SetRelativeLocation({ 0, 0, 0 });
		NewAttach->SetRelativeRotation({ 0, 0, 0 });
		NewAttach->RegisterComponent();
		GC_AttachPoints.Add(NewAttach);

		USMC_GlowyRotatingInclinedCube* NewCube = NewObject<USMC_GlowyRotatingInclinedCube>(this);
		NewCube->SetupAttachment(NewAttach);
		NewCube->SetRelativeLocation({ 0, 0, 0 });
		NewCube->SetRelativeScale3D({ 0.30f, 0.30f, 0.30f });
		NewCube->RegisterComponent();
		NewCube->NewTargetLocation({ 0, 0, 100.f });
		NewCube->Rotation = { FMath::FRandRange(FragmentLowerRotateSpeedRange, FragmentUpperRotateSpeedRange),
							  FMath::FRandRange(FragmentLowerRotateSpeedRange, FragmentUpperRotateSpeedRange),
							  FMath::FRandRange(FragmentLowerRotateSpeedRange, FragmentUpperRotateSpeedRange)  };

		GlowyCubes.Add(NewCube);

		for (int j = 0; j < GlowyCubes.Num(); j++)
		{
			GC_AttachPoints[j]->SetRelativeRotation({ 360.f * ((float)j / (float)GlowyCubes.Num()), 90.f, 0.f });
		}
	}
}

void AAIControlDirectorUnit::DestroyOldFragments()
{
	for (int i = 0; i < GlowyCubes.Num(); i++)
	{
		GC_AttachPoints[i]->DestroyComponent();

		GlowyCubes[i]->SetComponentTickEnabled(true);
		GlowyCubes[i]->SetSimulatePhysics(true);
		GlowyCubes[i]->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}

	GC_AttachPoints.Empty();
	GlowyCubes.Empty();
}


void AAIControlDirectorUnit::OrderAttack()
{
	if (ArcherAttacking >= ArcherAttackerLimit && MeleeAttacking >= MeleeAttackerLimit) return;

	TArray<int> Indexes{};
	for (int i = 0; i < EnemyRCPs.Num(); i++)
	{ 
		if (EnemyRCPs[i]->AIP_CanAttack()) 
		{ Indexes.AddUnique(i); }
	}


	while (Indexes.Num() > 0)
	{
		int RandomIndex = FMath::RandRange(0, Indexes.Num() - 1);

		UAIRemoteControlPlugin* ControlPlug = EnemyRCPs[Indexes[RandomIndex]];

		if		(ControlPlug->AIP_GetControlledType()->IsChildOf<ACE_MeleeEnemy>() && MeleeAttacking < MeleeAttackerLimit)
		{
			FPlugActionInfo&& Result = ControlPlug->AIP_StartAttack();
			
			if (Result.Success) 
			{ MeleeAttacking++; break; }
		}

		else if (ControlPlug->AIP_GetControlledType()->IsChildOf<ACE_ArcherEnemy>() && ArcherAttacking < ArcherAttackerLimit)
		{
			FPlugActionInfo&& Result = ControlPlug->AIP_StartAttack();

			if (Result.Success)
			{ ArcherAttacking++; break; }
		}

		Indexes.RemoveAt(RandomIndex);
	}
}

void AAIControlDirectorUnit::EnemyControlTick(float DeltaTime)
{
	if (EnemyRCPs.Num() > 0)
	{
		if (TimerBetweenEnemyOrders > 0) TimerBetweenEnemyOrders -= DeltaTime;
		else
		{
			TimerBetweenEnemyOrders = TimeBetweenEnemyOrders;

			OrderAttack();
		}
	}
}

void AAIControlDirectorUnit::WaveControlTick(float DeltaTime)
{
	if (!WaveActive && PhaseActive && !PreWaveActive)
	{
		if (TimerBetweenWaves > 0) TimerBetweenWaves -= DeltaTime;
		else if (GlowyCubes.Num() > 0)
		{
			MainSphere->SuddenColor({ 0, 10, 0, 1 });

			GlowyCubes[Local_CurrentWave]->SuddenGlow(10000.f);
			GlowyCubes[Local_CurrentWave]->NewColorTarget({ 1.f, 1.f,  1.f });

			WaveActive = true;

			for (AA_EnemySpawner* Spawner : EnemySpawners)
			{
				if (Spawner->WaveOrder(Global_CurrentWave))
					SpawnLightning(Spawner);
			}
		}
	}
}

void AAIControlDirectorUnit::PhaseControlTick(float DeltaTime)
{
	if ( !PhaseActive && (ArenasIn > 0 && PlayerInAnArena) )
	{
		if (NextPhaseTimer > 0) NextPhaseTimer -= DeltaTime;
		else
		{
			CreateNewFragments(PhasesAndWaves[CurrentPhase]);

			PhaseActive = true;
			WavesRemaining = PhasesAndWaves[CurrentPhase];

			DirectorBeganPhase();
		}
	}
}

void AAIControlDirectorUnit::EndWave()
{
	GlowyCubes[Local_CurrentWave]->SuddenGlow(10000.f);
	GlowyCubes[Local_CurrentWave]->NewGlowTarget(0.f);
	GlowyCubes[Local_CurrentWave]->NewColorTarget({ 0.1f, 0.1f, 0.1f });

	TimerBetweenWaves = TimeBetweenWaves;
	Global_CurrentWave++;
	WaveActive = false;

	WavesRemaining--;
}

void AAIControlDirectorUnit::EndPhase()
{
	NextPhaseTimer = NextPhaseTime;

	DestroyOldFragments();
	
	DirectorEndedPhase();


	if (PhasesAndWaves.IsValidIndex(CurrentPhase + 1))	// the AIcDu continues to next phase, the game keeps going!
	{
		CurrentPhase++;
		Local_CurrentWave = 0;
		PhaseActive = false;


		if (WayPoints.IsValidIndex(CurrentWayPoint + 1) && WayPoints[CurrentWayPoint]->CanMoveOn(1))
		{
			PlayerInAnArena = false;

			CurrentWayPoint++;
			Moving = true;
		}
	}
	else DirectorShutDown(); // This was the last Phase.
}


void AAIControlDirectorUnit::MovementTick(float DeltaTime)
{
	if (Moving)
	{
		FVector ToNextWP		= WayPoints[CurrentWayPoint]->GetActorLocation() - GetActorLocation();
		FVector NextMoveOffset	= ToNextWP.GetSafeNormal() * MovementSpeed * DeltaTime;

		if (ToNextWP.Size() < NextMoveOffset.Size())
		{
			WayPoints[CurrentWayPoint]->SnapIn(this);

			if (WayPoints.IsValidIndex(CurrentWayPoint + 1) && WayPoints[CurrentWayPoint]->CanMoveOn())
			{
				CurrentWayPoint++;
			}
			else Moving = false;
		}
		else AddActorWorldOffset(NextMoveOffset);
	}
}

void AAIControlDirectorUnit::DirectorShutDown()
{
	MainSphere->SetSimulatePhysics(true);
	MainSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	MainSphere->SuddenGlow(10000.f);
	MainSphere->NewGlowTarget(0.f);
	MainSphere->NewColorTarget({0.1f, 0.1f, 0.1f, 1.f});

	Active = false;

	DirectorShuttedDown( DirectorsActivity(GetWorld()) );
}

void AAIControlDirectorUnit::SpawnLightning(AActor* Target)
{
	UParticleSystemComponent* NewEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LightningParticleSystem, GetActorTransform());


	FParticleSysParam LightningSourceParams;

	LightningSourceParams.Name		= TEXT("Source");
	LightningSourceParams.ParamType = EParticleSysParamType::PSPT_Actor;
	LightningSourceParams.Actor		= this;


	FParticleSysParam LightningTargetParams;

	LightningSourceParams.Name		= TEXT("Target");
	LightningSourceParams.ParamType = EParticleSysParamType::PSPT_Actor;
	LightningSourceParams.Actor		= Target;


	NewEmitter->InstanceParameters.Add(LightningSourceParams),
	NewEmitter->InstanceParameters.Add(LightningTargetParams);

	Emitters.Add(NewEmitter);
	EmitterTimers.Add(LightningLifeTime);
}

void AAIControlDirectorUnit::EmittersLifetimeTick(float DeltaTime)
{
	for (float& AlifeTime : EmitterTimers) AlifeTime -= DeltaTime;

	for (int i = 0; i < Emitters.Num(); i++)
	{
		if (EmitterTimers[i] <= 0)
		{
			Emitters[i]->Complete();

			Emitters.RemoveAt(i);
			EmitterTimers.RemoveAt(i);

			i = -1;
		}
	}
}

bool AAIControlDirectorUnit::DirectorsActivity(const UWorld* World)
{
	TArray<AActor*> Directors;
	UGameplayStatics::GetAllActorsOfClass(World, TSubclassOf<AAIControlDirectorUnit>(), Directors );


	for (AActor* ADirector : Directors)
	{ if (!Cast<AAIControlDirectorUnit>(ADirector)->Active) return false; }


	return true;
}

void AAIControlDirectorUnit::AIcDuDebugDisplay()
{
	static int TotalWaves = 0;
	if (TotalWaves == 0) for (int i : PhasesAndWaves) TotalWaves += i;

	int ActiveEnemies = 0;
	for (UAIRemoteControlPlugin* RCP : EnemyRCPs) if (RCP->AIP_CanAttack()) ActiveEnemies++;

	GetDMM(GetWorld())->AddMessageOnTop(
		FDebugMessage( FColor::Green,
		
		FString::Printf(TEXT("=====[AIcDu %d]=====\n\
		Current phase: %d || Local wave:  %d / %d  || Enemies able to attack %d  ||\n\
		---------------- > ----------------- > ------------------------ >  Currently attacking enemies: %d\n\
		Total phases:    %d || Global wave: %d / %d || Registered enemies      %d  ||"), 
		
		GroupNumber,
		CurrentPhase+1, Local_CurrentWave+1, PhasesAndWaves[CurrentPhase], ActiveEnemies,  
		ArcherAttacking + MeleeAttacking,
		PhasesAndWaves.Num(), Global_CurrentWave+1, TotalWaves, EnemyRCPs.Num())
	));
}

void AAIControlDirectorUnit::BeginPlay()
{
	Super::BeginPlay();

	TimerBetweenEnemyOrders = TimeBetweenEnemyOrders;
	TimerBetweenWaves		= TimeBetweenWaves;
	NextPhaseTimer			= NextPhaseTimer;


	PathSanityCheck();

	if (WayPoints.Num() > 0) 
	{
		WayPoints[0]->SnapIn(this);
	}
}

// Called every frame
void AAIControlDirectorUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainSphere->Update(DeltaTime);

	for (int i = 0; i < GlowyCubes.Num(); i++)
	{
		GC_AttachPoints[i]->AddLocalRotation(OrbitalSpeeds * DeltaTime);

		GlowyCubes[i]->Update(DeltaTime);
	}

	EnemyControlTick(DeltaTime);
	WaveControlTick(DeltaTime);
	PhaseControlTick(DeltaTime);

	MovementTick(DeltaTime);

	EmittersLifetimeTick(DeltaTime);


	if (DEBUG_AIcDu) AIcDuDebugDisplay();
}

void AAIControlDirectorUnit::RCP_Register(UAIRemoteControlPlugin* NewEnemyRCP)
{ EnemyRCPs.AddUnique(NewEnemyRCP); }


void AAIControlDirectorUnit::RCP_Plugout(UAIRemoteControlPlugin* UnpluggedRCP)
{
	if (EnemyRCPs.RemoveSingle(UnpluggedRCP) > 0 && PhaseActive)
	{
		MainSphere->SuddenGlow(10000.f);
		MainSphere->SuddenColor(FLinearColor(1.f, 0.f, 0.f));

		if (EnemyRCPs.Num() == 0)
		{
			if (PreWaveActive)
			{ PreWaveActive = false; return; }

			EndWave();
			
			if (WavesRemaining == 0)
			{
				EndPhase();
			}
			else Local_CurrentWave++;
		}
	}
}


void AAIControlDirectorUnit::RCP_AttackOverNotification(const TSubclassOf<AC_Enemy>& EnemyType)
{
	if (EnemyType->StaticClass())
	{
		if (EnemyType->IsChildOf<ACE_MeleeEnemy>())
		{ MeleeAttacking--; }

		else if (EnemyType->IsChildOf<ACE_ArcherEnemy>())
		{ ArcherAttacking--; }
	}
}

void AAIControlDirectorUnit::ES_Register(AA_EnemySpawner* NewEnemySpawner)
{ EnemySpawners.AddUnique(NewEnemySpawner); }


void AAIControlDirectorUnit::NewWayPoint()
{ 
	ASC_AIcDuWayPoint* NewWP = GetWorld()->SpawnActor<ASC_AIcDuWayPoint>(GetActorLocation() + FVector(0.f, 0.f, 100.f), GetActorRotation());

	NewWP->RegisterAllComponents();
	
	if (WayPoints.Num() > 0)	
	{ NewWP->GiveLastWP(WayPoints.Top()); }
	
	WayPoints.Add(NewWP);


	for (ASC_AIcDuWayPoint* WP : WayPoints)
	{ WP->TogglePath(true); }
}

void AAIControlDirectorUnit::PopWayPoint()
{ 
	if (WayPoints.Num() > 0)
		WayPoints.Pop()->Destroy(); 
}

void AAIControlDirectorUnit::TogglePath()
{
	for (ASC_AIcDuWayPoint* WP : WayPoints)
	{ WP->TogglePath(); }
}

void AAIControlDirectorUnit::PathSanityCheck()
{
	for (int i = 0; i < WayPoints.Num(); i++ )
	{
		if (WayPoints[i] == nullptr)
		{	
			WayPoints.RemoveAt(i);

			i = -1;
		}
		else
		{
			WayPoints[i]->GiveLastWP( (WayPoints.IsValidIndex(i -1) ? WayPoints[i -1] : nullptr) );
		}
	}
}
