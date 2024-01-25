// Fill out your copyright notice in the Description page of Project Settings.


#include "BBT_StartAndWaitAttackCharge.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBBT_StartAndWaitAttackCharge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->BeginAttackCharge())
	{
		return Super::ExecuteTask(OwnerComp, NodeMemory);
	}
	else return EBTNodeResult::Failed;
}

void UBBT_StartAndWaitAttackCharge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AC_Enemy* Enemy = Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

	if (Enemy->Is_Hit())
	{
		Enemy->AbortAttackCycle();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
