// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()

	// Reference to the AIC's owned BehaviorTree.
	class UBehaviorTreeComponent* BehaviorComp;

	// Reference to the AIC's owned Blackboard.
	UBlackboardComponent* BlackboardComp;

	// Called on spawn of the enemy. It is a good "entry point" to initialize the blackboard and the behavior tree.
	virtual void OnPossess(APawn* Pawn) override;

protected:
	virtual void BeginPlay() override;

public:
	AAIC_Enemy();

	bool PlayerGet();

	bool ResumeBehaviorTree();

	void SetBBvalue	(const FName& _keyName, const bool _value);
	void SetBBvalue	(const FName& _keyName, const float _value);

	void EndBehaviorTree();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
