// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MoveTowardsPlayer.h"

#include "C_Enemy.h"
#include "HookNFightCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_MoveTowardsPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	AC_Enemy*				Enemy		= Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"));

	const FVector&& PlayerPos = Cast<AHookNFightCharacter>(Blackboard->GetValueAsObject("Player"))->GetActorLocation();
	const FVector&& EnemyPos = Enemy->GetActorLocation();

	Enemy->AddMovement((PlayerPos - EnemyPos).GetSafeNormal() * MovementAdded);
	
	return EBTNodeResult::Succeeded;
}
