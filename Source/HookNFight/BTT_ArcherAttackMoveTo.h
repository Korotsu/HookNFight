// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_EnemyAttackMoveTo.h"
#include "BTT_ArcherAttackMoveTo.generated.h"

/**
	A more aware MoveTo prepared for the Archer Enemy.
 */
UCLASS()
class HOOKNFIGHT_API UBTT_ArcherAttackMoveTo : public UBTT_EnemyAttackMoveTo
{
	GENERATED_BODY()
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
