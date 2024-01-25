// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTT_EnemyAttackMoveTo.generated.h"

/**
	A general yet more complex than usual moveto.
	Suitable for the melee enemy.
 */
UCLASS()
class HOOKNFIGHT_API UBTT_EnemyAttackMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;


	UPROPERTY(EditAnywhere, Category = Task, meta = (ClampMin = 0.f, UIMin = 1.f, UIMax = 200.f, ToolTip = "How much does encountering another Enemy while going towards the player would affect his path?"))
	float MovementAdded = 200.f;

	float InitialCapsuleRadius = 0.f;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
