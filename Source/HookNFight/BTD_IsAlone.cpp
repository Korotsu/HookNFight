// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsAlone.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_IsAlone::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->GetNearbyEnemies().Num() == 0;
}
