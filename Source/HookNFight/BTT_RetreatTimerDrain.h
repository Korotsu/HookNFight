// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RetreatTimerDrain.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBTT_RetreatTimerDrain : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	constexpr static float DeltaConversion = 6.f;
};
