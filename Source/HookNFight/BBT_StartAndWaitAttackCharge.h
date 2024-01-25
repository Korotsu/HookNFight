// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_WaitBlackboardTime.h"
#include "BBT_StartAndWaitAttackCharge.generated.h"

/**
	Possible scenarios :
	1. The player is not in range and the node returns a fail,
	2. The player is in range at the time of the call and the timer begins.
 */
UCLASS()
class HOOKNFIGHT_API UBBT_StartAndWaitAttackCharge : public UBTTask_WaitBlackboardTime
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
