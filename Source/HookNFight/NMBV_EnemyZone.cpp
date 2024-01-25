// Fill out your copyright notice in the Description page of Project Settings.


#include "NMBV_EnemyZone.h"

#include "HookNFightCharacter.h"
#include "AIControlDirectorUnit.h"
#include "C_Enemy.h"

#include "Components/BoxComponent.h"
#include "Components/BrushComponent.h"


ANMBV_EnemyZone::ANMBV_EnemyZone()
{
	GetBrushComponent()->SetCollisionProfileName("ArenaZone");
	BrushColor = FColor::Red;
}


const bool ANMBV_EnemyZone::ZonedEnemiesDead() const
{
	for (AC_Enemy* AEnemy : ZonedEnemies)
	{
		if (AEnemy->alive) return false;
	}

	return true;
}

void ANMBV_EnemyZone::GetReleventOverlappingElements()
{
	UpdateOverlaps();

	TArray<AActor*> OverlappingActors;

	GetOverlappingActors(OverlappingActors);

	for (AActor* AnActor : OverlappingActors)
	{
		AC_Enemy* AEnemy = Cast<AC_Enemy>(AnActor);

		if (AEnemy)
		{
			ZonedEnemies.AddUnique(AEnemy);
			AEnemy->Deactivate();
		}
	}

	for (AActor* AnActor : OverlappingActors)
		if (Cast<AAIControlDirectorUnit>(AnActor)) OnOverlapBegin(this, AnActor);

	for (AActor* AnActor : OverlappingActors)
		if (Cast<AHookNFightCharacter>(AnActor)) OnOverlapBegin(this, AnActor);
}


void ANMBV_EnemyZone::BeginPlay()
{
	Super::BeginPlay();

	GetReleventOverlappingElements();

	OnActorBeginOverlap.AddDynamic(this, &ANMBV_EnemyZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ANMBV_EnemyZone::OnOverlapEnd);
}

void ANMBV_EnemyZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AHookNFightCharacter>(OtherActor))
	{
		PlayerIn = true;
		for (AAIControlDirectorUnit* AAIcDu : Directors) AAIcDu->ArenaPlayerOverlap(ZonedEnemiesDead() == false);

		switch (ArenaTriggerMode)
		{
		case EEnemyZoneTriggerMode::ATM_PlayerTriggersAll:
			for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->Activate(true);
			break;

		default:
			for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->Activate(false);
			break;
		}
	}
	else
	{
		AAIControlDirectorUnit* AIcDu = Cast<AAIControlDirectorUnit>(OtherActor);
		if (AIcDu)
		{
			Directors.AddUnique(AIcDu);
			AIcDu->EnteredAnArena();

			if (PlayerIn) AIcDu->ArenaPlayerOverlap(ZonedEnemiesDead() == false);


			switch (ArenaTriggerMode)
			{
			case EEnemyZoneTriggerMode::ATM_AICDUclaimers:
				for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->EnsureGroupNumber(AIcDu->GroupNumber, true);
				break;

			case EEnemyZoneTriggerMode::ATM_AICDUrequired:
				for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->Reconnect(AIcDu->GroupNumber);
				break;

			default:
				break;
			}
		}
	}
}

void ANMBV_EnemyZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AHookNFightCharacter>(OtherActor))
	{
		PlayerIn = false;

		for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->Deactivate(true);
	}
	else
	{
		AAIControlDirectorUnit* AIcDu = Cast<AAIControlDirectorUnit>(OtherActor);
		if (AIcDu)
		{
			AIcDu->LeftAnArena();
			Directors.Remove(AIcDu);
		}
	}
}
