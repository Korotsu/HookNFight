// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CheckNearPlayer.h"

#include "C_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_CheckNearPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	return EBTNodeResult::Succeeded;
}
