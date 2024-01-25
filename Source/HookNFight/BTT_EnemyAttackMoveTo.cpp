// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttackMoveTo.h"

#include "CE_MeleeEnemy.h"
#include "AIC_Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_EnemyAttackMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AC_Enemy* Enemy = Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	
	Enemy->bUseControllerRotationYaw = true;

	UCapsuleComponent* EnemyCapsule = Enemy->GetCapsuleComponent();

	InitialCapsuleRadius = EnemyCapsule->GetUnscaledCapsuleRadius();
	EnemyCapsule->SetCapsuleRadius(EnemyCapsule->GetUnscaledCapsuleHalfHeight());


	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTT_EnemyAttackMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AC_Enemy* Enemy = Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

	if (Enemy->Is_Hit())
	{
		Enemy->Controller->StopMovement();
		Enemy->AbortAttackCycle();

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_EnemyAttackMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AC_Enemy* Enemy = Cast<AC_Enemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));

	Enemy->bUseControllerRotationYaw = false;

	Enemy->GetCapsuleComponent()->SetCapsuleRadius(InitialCapsuleRadius);


	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
