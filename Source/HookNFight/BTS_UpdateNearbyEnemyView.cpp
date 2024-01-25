// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateNearbyEnemyView.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTS_UpdateNearbyEnemyView::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->DetectNearbyEnemies();
}
