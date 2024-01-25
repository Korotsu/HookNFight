// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ArcherAttackMoveTo.h"

#include "CE_ArcherEnemy.h"
#include "AIC_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"


void UBTT_ArcherAttackMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	ACE_ArcherEnemy*		Enemy		= Cast<ACE_ArcherEnemy>(Blackboard->GetValueAsObject("SelfActor"));

	const FVector&& PlayerPos = Cast<AActor>(Blackboard->GetValueAsObject("Player"))->GetActorLocation();


	if ( (PlayerPos - Enemy->GetActorLocation()).Size() <= Enemy->FireRange->GetScaledSphereRadius() )
	{
		Enemy->Controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
