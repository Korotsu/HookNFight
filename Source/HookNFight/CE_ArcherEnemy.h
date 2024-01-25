// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Enemy.h"
#include "CE_ArcherEnemy.generated.h"

/**
 * 
 */
UCLASS(hidedropdown)
class HOOKNFIGHT_API ACE_ArcherEnemy : public AC_Enemy
{
	GENERATED_BODY()

	UPROPERTY(NoClear, EditAnywhere, BlueprintReadOnly, Category = "Enemy|Arrow", meta = (AllowPrivateAccess = "true", NoResetToDefault))
	TSubclassOf<class AA_Fireable> ArrowType;

	UPROPERTY(NoClear, EditAnywhere, BLueprintReadOnly, Category = "Enemy|Arrow", meta = (AllowPrivateAccess = "true", ClampMin = 0.f))
	float ArrowSpeed = 1000.f;


	UPROPERTY(NoClear, EditAnywhere, BlueprintReadOnly, Category = "Enemy|Sounds", meta = (AllowPrivateAccess = "true"))
	// When the archer fires an arrow, this sound will be played.
	USoundBase* ArrowFired;


	class AA_Fireable* CurrentArrow = nullptr;

	class AHookNFightCharacter* Player = nullptr;

public:
	ACE_ArcherEnemy();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* FireRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI|Attack system", meta = (ClampMin = 0.f, ToolTip = "The archer is gonna keep is arrow fully charged for this moment before executing his attack."))
	float FullChargeHold = 1.f;

public:
	// If in range to fire an arrow, the archer enemy will charge up an arrow.
	virtual bool BeginAttackCharge() override;

	// An attack is successfully executed as the enemy fires an arrow. (Never fails)
	virtual bool ExecuteAttack() override;


	// Gives back the first Element of your choice detected inside the range zone.
	template<class T>
	FORCEINLINE T* DetectUniqueInRange()
	{
		TArray<AActor*> NearbyActors;

		FireRange->GetOverlappingActors(NearbyActors, TSubclassOf<T>());

		for (AActor* A : NearbyActors)
		{
			T* AskedE = Cast<T>(A);

			if (AskedE) return AskedE;
		}

		return nullptr;
	}
};
