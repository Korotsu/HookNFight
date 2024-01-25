// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_EnemySpawner.generated.h"


USTRUCT()
struct FWaveKey
{
	GENERATED_BODY()

	UPROPERTY(NoClear, EditAnywhere, meta = (NoResetToDefault, UIMin = 0))
	// The wave to which the Spawner should react.
	int WaveNumber = -1;


	FORCEINLINE FWaveKey()
	{ WaveNumber = -1; }

	FORCEINLINE FWaveKey(int _WaveNumber)
	{ WaveNumber = _WaveNumber; }

	FORCEINLINE const bool operator==(const FWaveKey& other) const
	{ return WaveNumber == other.WaveNumber; }
};

FORCEINLINE uint32 GetTypeHash(const FWaveKey& Key)
{ return FCrc::MemCrc32(&Key, sizeof(FWaveKey)); }


USTRUCT()
struct FEnemiesToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (NoResetToDefault))
	// Array of Enemy's sub-classes, used for instant spawning.
	// In the context of in-game spawning, it is highly recommended to spawn the blueprint.
	TArray<TSubclassOf<class AC_Enemy>> EnemyToSpawn;
};


UCLASS()
class HOOKNFIGHT_API AA_EnemySpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	// The spawner will associate with an AICDU of the same group number.
	int GroupNumber = 0;


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = 0.f, UIMin = 50.f, AllowPrivateAccess = "true"))
	// Distance between two enemies spawned at the same time.
	float DoubleSpawnDistanciation = 80.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = 0.f, UIMin = 50.f, AllowPrivateAccess = "true"))
	// If a lot of enemies (>2) are spawned, they form a circle of the radius indicated here.
	float MultiSpawnDistanciation = 150.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	// A smoke that puffs in around newly spawned enemies.
	class UParticleSystem* SmokeParticleSystem;


	// Associated Director determined with the GroupNumber.
	class AAIControlDirectorUnit* LinkedDirector;

	// Ground right under the spawner.
	FHitResult GroundHit;

public:	
	// Sets default values for this actor's properties
	AA_EnemySpawner();

	UPROPERTY(EditAnywhere, Category = "Spawn flags", meta = (NoResetToDefault))
	/*
		Map of all waves the spawner should react to.
		The wave number (key), is the wave number it should react to.
		The array (value), are the enemies that will spawn on the indicated wave.
	*/
	TMap<FWaveKey, FEnemiesToSpawn> SpawnFlags;

private:
	// Immediate spawning command.
	// Spawns all enemies included inside the argument given.
	void Spawn(const FEnemiesToSpawn* ToSpawn);

	// Precise spawn function, called by Spawn().
	void IndividualSpawn(const TSubclassOf<class AC_Enemy>& SpawnType, const FVector&& TwoDPos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Usually called by the AI director to trigger the spawner.
	// The spawner will look through its spawn flags and if one is found the same wave key, then it will spawn enemies of this map entry.
	bool WaveOrder(int WaveNumber);
};
