// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTW_StrikeSpeedDilatedWait.h"

#include "CE_MeleeEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTW_StrikeSpeedDilatedWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent*	Blackboard = OwnerComp.GetBlackboardComponent();
	ACE_MeleeEnemy*			MeleeEnemy = Cast<ACE_MeleeEnemy>(Blackboard->GetValueAsObject("SelfActor"));
	
	if (MeleeEnemy)
	{
		FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;

		MyMemory->RemainingWaitTime = 
			FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation)) 
			* (1.f / MeleeEnemy->StrikeSpeed);

		return EBTNodeResult::InProgress;
	}
	else return EBTNodeResult::Succeeded;
}
