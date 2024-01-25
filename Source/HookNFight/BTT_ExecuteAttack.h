// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExecuteAttack.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBTT_ExecuteAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PostAttack", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, ToolTip = "Right after his attack, if the enemy can retreat he will do so in this amount of time."))
	float AfterAttackRetreatTimer = 0.3f;
};
