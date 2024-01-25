// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ComputeNextMoveDestination.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_ComputeNextMoveDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->ExecuteMovement();

	return EBTNodeResult::Succeeded;
}
