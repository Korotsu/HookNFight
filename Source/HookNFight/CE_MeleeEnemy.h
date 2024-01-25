// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Enemy.h"
#include "CE_MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS(hidedropdown)
class HOOKNFIGHT_API ACE_MeleeEnemy : public AC_Enemy
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI|Attack system", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	// Damage caused by an accomplished strike of the melee Enemy to the player.
	float DamagePower = 1;

public:
	ACE_MeleeEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI|Attack system", meta = (ClampMin = 0.f))
	// The total duration of the Melee Enemy's strike.
	float StrikeSpeed = 5.f;

public:
	// Once close to the Player, the melee enemy will charge up a sword strike.
	virtual bool BeginAttackCharge() override;

	// If the player is still right in front of the Enemy, an attack is successfully executed as the enemy hits the player.
	virtual bool ExecuteAttack() override;
};
