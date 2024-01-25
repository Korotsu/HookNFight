// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_WaitBlackboardTime.h"
#include "BTTW_EnemyPreCharge.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBTTW_EnemyPreCharge : public UBTTask_WaitBlackboardTime
{
	GENERATED_BODY()

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
