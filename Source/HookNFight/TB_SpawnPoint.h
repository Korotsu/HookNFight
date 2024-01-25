// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TB_SpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API ATB_SpawnPoint : public ATriggerBox
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "SpawnPoint")
		class USceneComponent* spawnPoint;

public:

	// constructor sets default values for this actor's properties
	ATB_SpawnPoint();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
