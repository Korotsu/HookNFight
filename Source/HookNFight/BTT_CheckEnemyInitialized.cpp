// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CheckEnemyInitialized.h"

#include "C_Enemy.h"
#include "HookNFightCharacter.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_CheckEnemyInitialized::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	UObject* Player = Blackboard->GetValueAsObject("Player");


	if (!Player) 
	{
		Blackboard->SetValueAsObject("Player", UGameplayStatics::GetActorOfClass(GetWorld(), AHookNFightCharacter::StaticClass()));
		return EBTNodeResult::Failed;
	}
	else return EBTNodeResult::Succeeded;
}
