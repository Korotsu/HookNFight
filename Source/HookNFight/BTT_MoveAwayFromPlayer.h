// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveAwayFromPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBTT_MoveAwayFromPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = Task, meta = (ClampMin = 0.f, UIMin = 1.f, UIMax = 200.f, ToolTip = "How much does this affects the next enemy's movement ?"))
	float MovementAdded = 25.f;
};
