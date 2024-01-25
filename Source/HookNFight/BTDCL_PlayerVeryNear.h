// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_ConditionalLoop.h"
#include "BTDCL_PlayerVeryNear.generated.h"

/**
 * 
 */
UCLASS(HideCategories = (Blackboard))
class HOOKNFIGHT_API UBTDCL_PlayerVeryNear : public UBTDecorator_ConditionalLoop
{
	GENERATED_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
};
