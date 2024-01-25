// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemyStatusDebug.generated.h"


UCLASS(meta = (BlueprintSpawnableComponent))
class HOOKNFIGHT_API UEnemyStatusDebug : public USceneComponent
{
	GENERATED_BODY()

public:
	UEnemyStatusDebug();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	class USMC_GlowySphere* ActionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	class USMC_GlowySphere* DirectorSphere;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
