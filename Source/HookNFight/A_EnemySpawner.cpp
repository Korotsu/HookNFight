// Fill out your copyright notice in the Description page of Project Settings.


#include "A_EnemySpawner.h"

#include "C_Enemy.h"
#include "AIControlDirectorUnit.h"

#include "Components/CapsuleComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AA_EnemySpawner::AA_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> baseSmokeParticleSystemAsset(TEXT("/Game/j_cellier/Ressources_Julie/Particule/Fumee/PFumee"));
	if (baseSmokeParticleSystemAsset.Object) SmokeParticleSystem = baseSmokeParticleSystemAsset.Object;
}

// Called when the game starts or when spawned
void AA_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Directors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIControlDirectorUnit::StaticClass(), Directors);

	for (AActor* Dir : Directors)
	{
		AAIControlDirectorUnit* ADirector = Cast<AAIControlDirectorUnit>(Dir);

		if (ADirector->GroupNumber == GroupNumber)
		{
			LinkedDirector = ADirector;
			LinkedDirector->ES_Register(this);
			break;
		}
	}

	GetWorld()->LineTraceSingleByChannel(GroundHit, GetActorLocation(), GetActorLocation() - FVector(0, 0, 1000.f), ECollisionChannel::ECC_WorldStatic);
}

void AA_EnemySpawner::Spawn(const FEnemiesToSpawn* ToSpawn)
{
	int NumOfEnemies = ToSpawn->EnemyToSpawn.Num();

	// Spawning is done right under the spawner
	if (NumOfEnemies == 1)
	{
		IndividualSpawn(ToSpawn->EnemyToSpawn[0], GetActorLocation());
	}

	// Spawning is done on a horizontal line
	else if (NumOfEnemies == 2)
	{
		IndividualSpawn(ToSpawn->EnemyToSpawn[0], GetActorLocation() - FVector(0.f, DoubleSpawnDistanciation, 0.f));
		IndividualSpawn(ToSpawn->EnemyToSpawn[1], GetActorLocation() + FVector(0.f, DoubleSpawnDistanciation, 0.f));
	}

	// Circular spawning
	else if (NumOfEnemies >= 3)
	{
		FVector&& SpawnPoint = FVector(MultiSpawnDistanciation, 0.f, 0.f);
		
		for (int i = 0; i < NumOfEnemies; i++)
		{
			IndividualSpawn(ToSpawn->EnemyToSpawn[i], GetActorLocation() + SpawnPoint);

			SpawnPoint = SpawnPoint.RotateAngleAxis(360.f/(float)NumOfEnemies, {0, 0, 1});
		}
	}
}

void AA_EnemySpawner::IndividualSpawn(const TSubclassOf<AC_Enemy>& SpawnType, const FVector&& TwoDPos)
{
	AC_Enemy* Enemy = GetWorld()->SpawnActor<AC_Enemy>(SpawnType);

	Enemy->RegisterAllComponents();
	
	// TODO: Fix the spawning location in Z, in accordance to the groundHit
	Enemy->SetActorLocationAndRotation(
		FVector	(TwoDPos.X, TwoDPos.Y, GroundHit.ImpactPoint.Z + Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0.f, GetActorRotation().Yaw, 0.f)
	);

	if ( Enemy->EnsureGroupNumber(GroupNumber, true) == true )
	{ Enemy->Reconnect(GroupNumber); }


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeParticleSystem, Enemy->GetActorTransform());
}

bool AA_EnemySpawner::WaveOrder(int WaveNumber)
{
	FEnemiesToSpawn* WaveSpawn = SpawnFlags.Find(FWaveKey(WaveNumber));

	if (WaveSpawn) 
	{
		Spawn(WaveSpawn);
		return true;
	}
	else return false;
}

