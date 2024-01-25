// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTW_StrikeSpeedDilatedWait.generated.h"

/**
	Will be skipped automatically if not suitable.
 */
UCLASS()
class HOOKNFIGHT_API UBTTW_StrikeSpeedDilatedWait : public UBTTask_Wait
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
