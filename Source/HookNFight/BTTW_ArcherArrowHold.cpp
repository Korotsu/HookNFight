// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTW_ArcherArrowHold.h"

#include "AIC_Enemy.h"
#include "CE_ArcherEnemy.h"
#include "HookNFightCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTW_ArcherArrowHold::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime = Cast<ACE_ArcherEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"))->FullChargeHold;

	return EBTNodeResult::InProgress;
}

void UBTTW_ArcherArrowHold::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	ACE_ArcherEnemy*		Enemy		= Cast<ACE_ArcherEnemy>(Blackboard->GetValueAsObject("SelfActor"));

	const FVector&& PlayerPos = Cast<AHookNFightCharacter>(Blackboard->GetValueAsObject("Player"))->GetActorLocation();
	const FVector&& EnemyPos = Enemy->GetActorLocation();

	float&& TurnFactor = (Enemy->FullChargeHold - ((FBTWaitTaskMemory*)NodeMemory)->RemainingWaitTime)/ Enemy->FullChargeHold;
	Enemy->SetActorRotation(FMath::Lerp(Enemy->GetActorRotation(), (PlayerPos - EnemyPos).GetSafeNormal2D().Rotation(), TurnFactor));

	
	if (Enemy->Is_Hit())
	{
		Enemy->AbortAttackCycle();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
