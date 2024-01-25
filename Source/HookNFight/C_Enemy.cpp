// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Enemy.h"

#include "AIC_Enemy.h"
#include "AIRemoteControlPlugin.h"
#include "HookNFightCharacter.h"
#include "BPaCpp_FunctionLibrary.h"

#include "TimerManager.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"

#include "Materials/Material.h"

#include "Engine/World.h"
#include "Engine/Engine.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AC_Enemy::AC_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/*(Further settings for the WeaponMesh are applied in sub-classes)*/

	static ConstructorHelpers::FObjectFinder<UMaterial> baseMaterialAsset(TEXT("/Game/I_bessy/I_bessy/Characters/Ennemies/Materials/M_PolyartEnnemy"));
	if (baseMaterialAsset.Succeeded())
	{
		GetMesh()->SetMaterial(0, baseMaterialAsset.Object);
		WeaponMesh->SetMaterial(0, baseMaterialAsset.Object);
	}

	CloseZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CloseZone"));
	CloseZone->SetupAttachment(RootComponent);
	CloseZone->SetRelativeLocation({ 80, 0, -50 });
	CloseZone->SetBoxExtent({ 75, 75, 10 }, false);
	CloseZone->SetCollisionProfileName("CloseZone");

	PlayerKeepAwayZone = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerKeepAwayZone"));
	PlayerKeepAwayZone->SetupAttachment(RootComponent);
	PlayerKeepAwayZone->SetRelativeLocation({ 0, 0, 0 });
	PlayerKeepAwayZone->SetSphereRadius(400.f);
	PlayerKeepAwayZone->SetGenerateOverlapEvents(false);
	PlayerKeepAwayZone->SetCollisionProfileName("DetectionZone");

	soundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundPlayer"));
	soundPlayer->SetupAttachment(RootComponent);
	soundPlayer->SetRelativeLocation({ 0, 0, 0 });


	GetMesh()->SetRelativeLocationAndRotation({0.f, 0.f, -90.f}, { 0.f, -90.f, 0.f});
	GetMesh()->SetCollisionProfileName("Enemy");

	GetCapsuleComponent()->SetCollisionProfileName("Enemy");


	RemoteControlPlugin = CreateDefaultSubobject<UAIRemoteControlPlugin>(TEXT("RemoteControlPlugin"));

	AIControllerClass = TSubclassOf<AAIC_Enemy>();

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AC_Enemy::Set_Grabbed(const bool Value)
{
	if (this)
	{
		if (Value == true) Controller->StopMovement();

		Grabbed = Value;
	}
}

const FVector AC_Enemy::SafeScale_MoveBy()
{
	if (!MoveBy.IsNearlyZero())
	{
		const FVector&& MB = MoveBy.GetSafeNormal() * (GetCharacterMovement()->MaxWalkSpeed/6.f);
		MoveBy = FVector::ZeroVector;

		return MB;
	}
	else return FVector::ZeroVector;
}

void AC_Enemy::EndHit()
{
	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(Controller);
	
	if (life > 0)
	{
		AIController->ResumeBehaviorTree();

		hitState = false;

		AIController->SetBBvalue(TEXT("Retreat"), true);
		AIController->SetBBvalue(TEXT("RetreatTimer"), 0.f);
	}
	else if (alive)
	{
		if (Attacking) RemoteControlPlugin->AIP_NotifyEndAttack();
		RemoteControlPlugin->Unplug();

		AIController->EndBehaviorTree();

		SetActorEnableCollision(false);

		alive = false;
		Active = false;
	}
}

void AC_Enemy::PlayNoise(USoundBase* Sound)
{
	soundPlayer->SetSound(Sound);
	soundPlayer->Play();
}

void AC_Enemy::Activate(bool Replug)
{
	if (alive)
	{
		checkf(Active == false, TEXT("Attempted to activate an enemy already activated."))


		Active = true;

		if (Replug == true) RemoteControlPlugin->Replug();
	}
}


bool AC_Enemy::Activate(const int& NewGroupNumber)
{
	if (alive)
	{
		checkf(Active == false, TEXT("Attempted to activate an enemy already activated."))
	

		Active = true;

		return RemoteControlPlugin->Replug(NewGroupNumber);
	} 
	else return false;
}

void AC_Enemy::Deactivate(bool Unplug)
{
	if (alive)
	{
		checkf(Active == true, TEXT("Attempted to activate an enemy already deactivated."))
		
		
		AbortAttackCycle();

		if (Unplug == true) RemoteControlPlugin->Unplug();

		Active = false;
	}
}

void AC_Enemy::Reconnect(const int& TestGroupNumber)
{ if (RemoteControlPlugin->Get_GroupNumber() == TestGroupNumber) RemoteControlPlugin->Replug(); }


const bool AC_Enemy::EnsureGroupNumber(const int& DesiredGroupNumber, const bool&& ReplugAuthorized)
{
	if (RemoteControlPlugin->Get_GroupNumber() == DesiredGroupNumber)
	{ return true; }

	else 
	{
		if (ReplugAuthorized && alive) RemoteControlPlugin->Replug(DesiredGroupNumber);

		return false;
	}
}

void AC_Enemy::ExecuteMovement()
{
	if (!Grabbed && hitState == false) 
	{
		Cast<AAIC_Enemy>(Controller)->MoveToLocation(GetActorLocation() + SafeScale_MoveBy(), -1.f, false, true, true, false);
	}
	else 
	{ MoveBy = FVector::ZeroVector; }
}

float AC_Enemy::CalculateChargeTime(const FVector& PlayerPos, const FVector& PlayerForward)
{
	FVector&& PlayerToEnemy = GetActorLocation() - PlayerPos;

	AttackChargeTimer =
	( UBPaCpp_FunctionLibrary::VectorAngle(PlayerToEnemy, PlayerForward, {0, 0, 1}) > 90.f) ? AttackChargeTime + AttackBackstabDelay : AttackChargeTime;
	
	return AttackChargeTimer;
}

void AC_Enemy::Get_Hit(float Damage)
{
	if (life > 0)
	{
		hitState = true;
		life -= Damage;

		UBPaCpp_FunctionLibrary::TriggerAnimBPEvent(GetMesh()->GetAnimInstance(), TEXT("EnemyNotify_GotHit"));

		StunTimeHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(StunTimeHandle, this, &AC_Enemy::EndHit, StunTime);
	}
}

void AC_Enemy::AbortAttackCycle()
{
	if (Attacking)
	{
		RemoteControlPlugin->AIP_NotifyEndAttack();
		Attacking = false;
	}

	if (ChargingAttack) ChargingAttack = false;
}

void AC_Enemy::DetectNearbyEnemies()
{
	TArray<AActor*> NearbyEnemiesNC;

 	CloseZone->GetOverlappingActors(NearbyEnemiesNC, AC_Enemy::StaticClass());
	NearbyEnemiesNC.Remove(this);


	if (NearbyEnemies.Num() > 0) 
		NearbyEnemies.Empty();

	for (AActor* ClsA : NearbyEnemiesNC)
		NearbyEnemies.Add(Cast<AC_Enemy>(ClsA));
}

