// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LookAtPlayer.generated.h"

UENUM(BlueprintType)
enum class ERotationType : uint8
{
	RT_instant		UMETA(DisplayName = "Instantaneous"),
	RT_progressive	UMETA(DisplayName = "Progressive"),
	RT_donot		UMETA(DisplayName = "No rotation")
};


UCLASS()
class HOOKNFIGHT_API UBTT_LookAtPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, Category = Task, meta = (ToolTip = "Controls how the enemy should turn around to face the player."))
	ERotationType RotationMode = ERotationType::RT_instant;

	UPROPERTY(EditAnywhere, Category = Task, meta = (ClampMin = 0.f, UIMin = 0.05f, UIMax = 0.95f, ClampMax = 1.f, ToolTip = "How fast does the enemy turns around ? (No effect if rotation is \"instantanious\" or disabled.)"))
	float RotationSpeed = 0.5f;

};
