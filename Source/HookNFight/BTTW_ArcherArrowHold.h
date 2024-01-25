// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTW_ArcherArrowHold.generated.h"

/**
	A special wait designed for the archer. He turn to face the player progressively during this time.
 */
UCLASS(hidecategories=Wait)
class HOOKNFIGHT_API UBTTW_ArcherArrowHold : public UBTTask_Wait
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
