// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_EnemyActive.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UBTD_EnemyActive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->Is_Active();
}
