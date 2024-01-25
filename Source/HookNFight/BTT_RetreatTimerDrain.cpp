// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RetreatTimerDrain.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_RetreatTimerDrain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	AC_Enemy*				Enemy		= Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"));
	
	if (!Blackboard->GetValueAsBool("Retreat") && Enemy->Get_CanRetreat())
	{
		Blackboard->SetValueAsBool("Retreat", true);
		Blackboard->SetValueAsFloat("RetreatTimer", Enemy->Get_RetreatDelay());
	}
	
	float NewTimer = Blackboard->GetValueAsFloat("RetreatTimer") - GetWorld()->GetDeltaSeconds() * DeltaConversion;

	Blackboard->SetValueAsFloat("RetreatTimer", NewTimer);


	if (NewTimer > 0)	return EBTNodeResult::Failed;
	else				return EBTNodeResult::Succeeded;
}
