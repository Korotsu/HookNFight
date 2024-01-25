// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_ShouldAttack.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBTD_ShouldAttack : public UBTDecorator
{
	GENERATED_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
