// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TB_KillZone.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API ATB_KillZone : public ATriggerBox
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		/** Damage inflict to the player. */
		float damage = 0;
public:

	// constructor sets default values for this actor's properties
	ATB_KillZone();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
};
