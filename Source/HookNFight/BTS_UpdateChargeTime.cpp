// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateChargeTime.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTS_UpdateChargeTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	AActor*					PlayerActor	= Cast<AActor>(Blackboard->GetValueAsObject("Player"));

	Blackboard->SetValueAsFloat( "ChargeTime", 
		Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"))->CalculateChargeTime(PlayerActor->GetActorLocation(), PlayerActor->GetActorForwardVector())
	);
}
