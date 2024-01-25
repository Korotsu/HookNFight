// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTW_EnemyPreCharge.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTTW_EnemyPreCharge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->Is_Hit())
	{
		OwnerComp.PauseLogic(TEXT("Enemy was hit, stunning in progress..."));
	}

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
