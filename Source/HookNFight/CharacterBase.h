// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS(hidedropdown, Abstract)
class HOOKNFIGHT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void Do_Attack(class UStaticMeshComponent* hitbox, float timeBeforeActivation, float damage) {}

	float timeSinceAttackStarted = 0;


	UPROPERTY(BlueprintReadWrite, Category = State)
	bool hitState = false;

	bool alreadyHit	= false;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats, meta = (UIMin = 0.f))
	float life = 10;

	UPROPERTY(BlueprintReadWrite, Category = "Sound")
	class UAudioComponent* soundPlayer;

	UFUNCTION(BlueprintCallable)
	void SetEffectVolume(float value);

public:

	float effectVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	bool alive = true;

	virtual void Get_Hit(float damage) {}
};
