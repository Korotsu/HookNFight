// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SC_AIcDuWayPoint.generated.h"

UCLASS(hideCategories = (Materials))
class HOOKNFIGHT_API ASC_AIcDuWayPoint : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USMC_GlowySphere* RootTransformComp;


	UPROPERTY(EditAnywhere, Category = "AICDU Path", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMax = 10.f))
	float TargetScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "AICDU Path", meta = (AllowPrivateAccess = "true"))
	FRotator TargetLookat;

	UPROPERTY(EditAnywhere, Category = "AICDU Path", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	int HaltForNPhases = 0;

	UPROPERTY(VisibleAnywhere, SaveGame, Category = "AICDU Path", meta = (AllowPrivateAccess = "true"))
	AActor* PreviousWayPoint;

	bool PathVisible = false;

public:	
	// Sets default values for this actor's properties
	ASC_AIcDuWayPoint();

	FORCEINLINE void TogglePath()				{ PathVisible ^= true;  }
	FORCEINLINE void TogglePath(bool status)	{ PathVisible = status; }

public:
	FORCEINLINE void GiveLastWP(AActor* _PreviousWayPoint)
	{ PreviousWayPoint = _PreviousWayPoint; }
	
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	// Removes phases (or not) from the countdown and returns if the director is now allowed to move.
	const bool CanMoveOn(int PhasesDone);

	FORCEINLINE bool CanMoveOn() { return HaltForNPhases <= 0; }

	void SnapIn(class AActor* Director);
};
