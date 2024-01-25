// Fill out your copyright notice in the Description page of Project Settings.


#include "CE_MeleeEnemy.h"

#include "AIRemoteControlPlugin.h"
#include "HookNFightCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "UObject/ConstructorHelpers.h"


ACE_MeleeEnemy::ACE_MeleeEnemy()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseWeaponMeshAsset(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM"));
	if (baseWeaponMeshAsset.Succeeded()) WeaponMesh->SetStaticMesh(baseWeaponMeshAsset.Object);

	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	WeaponMesh->SetRelativeLocationAndRotation({ -10.f, 5.f, 0 }, { 0, 0, -90.f });


	static ConstructorHelpers::FObjectFinder<UBehaviorTree> baseBehaviorTreeAsset(TEXT("/Game/h_chemouny/AI/BT_MeleeEnemy"));
	if (baseBehaviorTreeAsset.Succeeded()) BehaviorTree = baseBehaviorTreeAsset.Object;

	PlayerKeepAwayZone->SetSphereRadius(700.f);

	AttackChargeTime = 0.5f;
	AttackBackstabDelay = 0.5f;

	life = 6.f;
}

bool ACE_MeleeEnemy::BeginAttackCharge()
{
	if (DetectUnique<AHookNFightCharacter>())
	{
		ChargingAttack = true;
		return true;
	}
	else 
	{
		if (Attacking)
		{
			RemoteControlPlugin->AIP_NotifyEndAttack();
			Attacking = false;
		}

		return false;
	}
	
}

bool ACE_MeleeEnemy::ExecuteAttack()
{
	if (Attacking) RemoteControlPlugin->AIP_NotifyEndAttack();
	Attacking = false, ChargingAttack = false;

	AHookNFightCharacter* Player = DetectUnique<AHookNFightCharacter>();
	
	if (Player)
	{
		Player->Get_Hit(DamagePower);

		return true;
	}
	else return false;
}
