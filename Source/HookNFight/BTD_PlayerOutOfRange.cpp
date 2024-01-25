// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_PlayerOutOfRange.h"

#include "C_Enemy.h"

#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_PlayerOutOfRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	AC_Enemy*				Enemy		= Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"));

	const FVector&& PlayerPos = Cast<AActor>(Blackboard->GetValueAsObject("Player"))->GetActorLocation();

	
	return ((PlayerPos - Enemy->GetActorLocation()).Size() > (Enemy->PlayerKeepAwayZone->GetScaledSphereRadius() + Enemy->Tolerance) ? true : false);
}
