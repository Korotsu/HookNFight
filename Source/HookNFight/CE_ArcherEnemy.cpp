// Fill out your copyright notice in the Description page of Project Settings.


#include "CE_ArcherEnemy.h"

#include "AIRemoteControlPlugin.h"
#include "A_Fireable.h"
#include "HookNFightCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"


ACE_ArcherEnemy::ACE_ArcherEnemy()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseWeaponMeshAsset(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Bow01SM"));
	if (baseWeaponMeshAsset.Succeeded()) WeaponMesh->SetStaticMesh(baseWeaponMeshAsset.Object);

	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	WeaponMesh->SetRelativeLocationAndRotation({ 10.f, -5.f, 0 }, { 0, 0, 90.f });


	FireRange = CreateDefaultSubobject<USphereComponent>(TEXT("RangeOfFire"));
	FireRange->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FireRange->SetRelativeLocation({ 0, 0, 0 });
	FireRange->SetSphereRadius(2500.f);
	FireRange->SetCollisionProfileName("DetectionZone");

	PlayerKeepAwayZone->SetSphereRadius(800.f);

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> baseBehaviorTreeAsset(TEXT("/Game/h_chemouny/AI/BT_ArcherEnemy"));
	if (baseBehaviorTreeAsset.Succeeded()) BehaviorTree = baseBehaviorTreeAsset.Object;

	life = 3;
}


bool ACE_ArcherEnemy::BeginAttackCharge()
{
	Player = nullptr;
	Player = DetectUniqueInRange<AHookNFightCharacter>();

	if (Player)
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

bool ACE_ArcherEnemy::ExecuteAttack()
{
	if (Attacking) RemoteControlPlugin->AIP_NotifyEndAttack();
	Attacking = false, ChargingAttack = false;

	AA_Fireable* Arrow = GetWorld()->SpawnActor<AA_Fireable>(ArrowType, GetActorLocation(), GetActorRotation());
	Arrow->RegisterAllComponents();
	Arrow->Fire( (Player->GetActorLocation() - GetActorLocation()), ArrowSpeed );

	PlayNoise(ArrowFired);

	return true;
}