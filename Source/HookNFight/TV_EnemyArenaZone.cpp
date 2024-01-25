// Fill out your copyright notice in the Description page of Project Settings.


#include "TV_EnemyArenaZone.h"

#include "HookNFightCharacter.h"
#include "AIControlDirectorUnit.h"
#include "C_Enemy.h"


ATV_EnemyArenaZone::ATV_EnemyArenaZone()
{
		
}

const bool ATV_EnemyArenaZone::ZonedEnemiesDead() const
{
	for (AC_Enemy* AEnemy : ZonedEnemies)
	{ if (AEnemy->alive) return false; }

	return true;
}

void ATV_EnemyArenaZone::GetReleventOverlappingElements()
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


void ATV_EnemyArenaZone::BeginPlay()
{
	Super::BeginPlay();

	GetReleventOverlappingElements();

	OnActorBeginOverlap.AddDynamic(this, &ATV_EnemyArenaZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATV_EnemyArenaZone::OnOverlapEnd);
}

void ATV_EnemyArenaZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AHookNFightCharacter>(OtherActor))
	{
		PlayerIn = true;
		for (AAIControlDirectorUnit* AAIcDu : Directors) AAIcDu->ArenaPlayerOverlap(ZonedEnemiesDead() == false);

		switch (ArenaTriggerMode)
		{
		case EArenaTriggerMode::ATM_PlayerTriggersAll:
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
			case EArenaTriggerMode::ATM_AICDUclaimers:
				for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->EnsureGroupNumber(AIcDu->GroupNumber, true);
				break;

			case EArenaTriggerMode::ATM_AICDUrequired:
				for (AC_Enemy* AEnemy : ZonedEnemies) AEnemy->Reconnect(AIcDu->GroupNumber);
				break;

			default:
				break;
			}
		}
	}
}

void ATV_EnemyArenaZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
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
