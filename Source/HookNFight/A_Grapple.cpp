// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Grapple.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "C_Enemy.h"

// Called when the game starts or when spawned
AA_Grapple::AA_Grapple()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	UStaticMeshComponent* grappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	grappleMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> grappleVisualAsset(TEXT("/Game/w_nardone/Game/Grapple/Meshes/Grapple"));
	if (grappleVisualAsset.Succeeded())
	{
		grappleMesh->SetStaticMesh(grappleVisualAsset.Object);
		UE_LOG(LogTemp, Warning, TEXT("MeshConstructed !"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshConstruction failed!"));
	}

	soundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundPlayer"));
	soundPlayer->SetupAttachment(RootComponent);

	grabPlayer = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> grappleTowingSound(TEXT("/Game/w_nardone/Game/Grapple/Sounds/towingSound"));
	if (grappleTowingSound.Succeeded())
	{
		towingSound = (grappleTowingSound.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> grappleSoundLoad(TEXT("/Game/w_nardone/Game/Grapple/Sounds/fireGrappleSound"));
	if (grappleSoundLoad.Succeeded())
	{
		grappleSound = (grappleSoundLoad.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> grappleCatchSound(TEXT("/Game/w_nardone/Game/Grapple/Sounds/catchSound"));
	if (grappleCatchSound.Succeeded())
	{
		catchSound = (grappleCatchSound.Object);
	}

	rope = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	rope->SetupAttachment(RootComponent);

	timeAfterLastTowingSound	= 0;
	timeAfterLastGrappleSound	= 0;
	grabRangeOffSet				= 10;
	goForward					= true;
	goBackward					= false;
}

void AA_Grapple::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_Grapple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (currentVolume != parent->effectVolume)
	{
		currentVolume = parent->effectVolume;
		soundPlayer->SetVolumeMultiplier(currentVolume);
	}

	FVector targetDirection = target->GetActorLocation() - base;

	SetActorRotation(targetDirection.Rotation());
	targetDirection.Normalize();

	FCollisionQueryParams mask = FCollisionQueryParams("test", false, parent);
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(hit, base, GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel8, mask);

	if (hit.GetActor() == nullptr && goBackward == false)
	{

		if (grabPlayer == true)
		{
			if (GetWorld()->GetRealTimeSeconds() - timeAfterLastTowingSound >= towingSound->GetDuration())
			{
				soundPlayer->SetSound(towingSound);
				soundPlayer->Play();
				timeAfterLastTowingSound = GetWorld()->GetRealTimeSeconds();
			}

			FVector path = GetActorLocation() - parent->GetActorLocation();
			path.Normalize();
			FVector rotation = path;
			rotation.Z = 0;

			parent->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			parent->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Ignore);

			if (path.Z > 0)
			{
				parent->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			}

			else
			{
				parent->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}

			parent->SetActorLocationAndRotation(parent->GetActorLocation() + path * towingSpeed * DeltaTime * 100, rotation.Rotation(), false, nullptr, ETeleportType::TeleportPhysics);
			//parent->AddMovementInput(path, 1 * DeltaTime, true);

			if ( FVector::Dist(parent->GetActorLocation(),GetActorLocation()) <= (towingSpeed * DeltaTime * 100) + grabRangeOffSet)
			{
				if (parent != nullptr)
				{
					parent->alreadyLaunchedGrapple = false;
					parent->grappleMesh->SetHiddenInGame(false);

					parent->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					parent->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
					parent->SetTarget(nullptr);
					parent->towed = false;

					parent->immune = false;
					parent->ImmuneHighlight(false);

					parent->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Block);
				}
				soundPlayer->Stop();

				AC_Enemy* enemy = Cast<AC_Enemy>(target);

				if (enemy)
				{
					enemy->Set_Grabbed(false);
					parent->GrappleAttack();
				}

				Destroy();
			}
		}

		else if (goForward == true)
		{
			if (GetWorld()->GetRealTimeSeconds() - timeAfterLastGrappleSound >= grappleSound->GetDuration())
			{
				soundPlayer->SetSound(grappleSound);
				soundPlayer->Play();
				timeAfterLastGrappleSound = GetWorld()->GetRealTimeSeconds();
			}

			if (FVector::Dist(GetActorLocation(), target->GetActorLocation()) <= (speed * DeltaTime * 100) * 2)
			{
				soundPlayer->Stop();
				soundPlayer->SetSound(catchSound);
				soundPlayer->Play();
				goForward = false;
				grabPlayer = true;
				parent->towed = true;
			}

			else
			{
				SetActorLocation(GetActorLocation() + targetDirection * (speed * DeltaTime * 100));
			}
		}
	}

	else
	{
		goBackward = true;

		if (FVector::Dist(base,GetActorLocation()) <= (speed * DeltaTime * 100) * 2)
		{
			if (parent != nullptr)
			{
				parent->alreadyLaunchedGrapple = false;
				parent->grappleMesh->SetHiddenInGame(false);

				parent->immune = true;
				parent->ImmuneHighlight(true);

				parent->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
				parent->SetTarget(nullptr);
			}
			soundPlayer->Stop();

			AC_Enemy* enemy = Cast<AC_Enemy>(target);

			if (enemy)
			{
				enemy->Set_Grabbed(false);
			}

			Destroy();
		}

		else
		{
			SetActorLocation(GetActorLocation() - (targetDirection * (speed * DeltaTime * 100)));
		}
	}

}

AA_Grapple* AA_Grapple::Launch(AActor* target, float speed, float towSpeed, UWorld* world, AHookNFightCharacter* parent)
{
	AC_Enemy* enemy = Cast<AC_Enemy>(target);

	if (enemy)
	{
		enemy->Set_Grabbed(true);
	}

	FVector targetDirection		= target->GetActorLocation() - parent->GetMesh()->GetSocketLocation("hand_lSocket");
	targetDirection.Normalize();

	parent->immune				= true;
	parent->ImmuneHighlight(true);

	AA_Grapple* grapple			= world->SpawnActor<AA_Grapple>(parent->GetMesh()->GetSocketLocation("hand_lSocket"), targetDirection.Rotation());
	grapple->target				= target;
	grapple->base				= parent->GetMesh()->GetSocketLocation("hand_lSocket");
	grapple->goForward			= true;
	grapple->speed				= speed;
	grapple->towingSpeed		= towSpeed;
	grapple->parent				= parent;
	grapple->rope->SetAttachEndTo(parent,"baseMesh");
	grapple->rope->EndLocation	= { 0,0,0 };
	grapple->grabRangeOffSet	= parent->grabRangeOffSet;
	parent->grappleMesh->SetHiddenInGame(true);
	
	return grapple;
}