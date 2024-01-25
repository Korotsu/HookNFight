// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Enemy.h"

#include "C_Enemy.h"
#include "HookNFightCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AAIC_Enemy::AAIC_Enemy()
{
	BehaviorComp	= CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));
	BlackboardComp	= CreateDefaultSubobject<UBlackboardComponent>	(TEXT("Blackboard"));
}

bool AAIC_Enemy::PlayerGet()
{
	BlackboardComp->SetValueAsObject("Player", UGameplayStatics::GetActorOfClass(GetWorld(), AHookNFightCharacter::StaticClass()));

	return BlackboardComp->GetValueAsObject("Player") != nullptr;
}

bool AAIC_Enemy::ResumeBehaviorTree()
{
	if (BehaviorComp->IsPaused()) 
	{
		BehaviorComp->ResumeLogic(FString(TEXT("StunEnd")));
		return true;
	}
	else return false;
}

void AAIC_Enemy::SetBBvalue(const FName& _keyName, const bool _value)
{ BlackboardComp->SetValueAsBool(_keyName, _value); }

void AAIC_Enemy::SetBBvalue(const FName& _keyName, const float _value)
{ BlackboardComp->SetValueAsFloat(_keyName, _value); }

void AAIC_Enemy::EndBehaviorTree()
{ BehaviorComp->StopTree(); }


void AAIC_Enemy::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	// We check that the character the AIC is attempting to possess is a C_Enemy. Otherwise they are incompatible.
	AC_Enemy* EnemyChar = Cast<AC_Enemy>(PossessedPawn);

	if (EnemyChar)
	{
		// If a valid blackboard was given, we can initialize it for the behavior tree.
		if (EnemyChar->BehaviorTree->BlackboardAsset)
			BlackboardComp->InitializeBlackboard(*(EnemyChar->BehaviorTree->BlackboardAsset));

		// Our EnemyChar's behavior tree is now ready, so we can start it here.
		BehaviorComp->StartTree(*EnemyChar->BehaviorTree);

		BlackboardComp->SetValueAsObject( "SelfActor"		, EnemyChar );
		BlackboardComp->SetValueAsFloat( "PreChargeWait"	, EnemyChar->PreAttackChargeDelay );
	}
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerGet();
}