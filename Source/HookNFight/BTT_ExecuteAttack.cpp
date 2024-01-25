// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ExecuteAttack.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_ExecuteAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"))->ExecuteAttack();
	
	Blackboard->SetValueAsFloat("RetreatTimer", AfterAttackRetreatTimer);
	Blackboard->SetValueAsBool("Retreat", true);

	return EBTNodeResult::Succeeded;
}
