// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LookAtPlayer.h"

#include "C_Enemy.h"
#include "HookNFightCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (RotationMode == ERotationType::RT_donot) return EBTNodeResult::Succeeded;

	
	UBlackboardComponent*	Blackboard	= OwnerComp.GetBlackboardComponent();
	AC_Enemy*				Enemy		= Cast<AC_Enemy>(Blackboard->GetValueAsObject("SelfActor"));

	const FVector&& PlayerPos = Cast<AHookNFightCharacter>(Blackboard->GetValueAsObject("Player"))->GetActorLocation();
	const FVector&& EnemyPos = Enemy->GetActorLocation();
	

	if		(RotationMode == ERotationType::RT_instant)
	{
		Enemy->SetActorRotation((PlayerPos - EnemyPos).GetSafeNormal2D().Rotation());
	}
	else if (RotationMode == ERotationType::RT_progressive)
	{
		Enemy->SetActorRotation(FMath::Lerp(Enemy->GetActorRotation(), (PlayerPos - EnemyPos).GetSafeNormal2D().Rotation(), RotationSpeed));
	}

	return EBTNodeResult::Succeeded;
}
