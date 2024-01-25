// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NMBV_EnemyZone.generated.h"


UENUM(BlueprintType)
enum class EEnemyZoneTriggerMode : uint8
{
	ATM_AICDUclaimers		UMETA(DisplayName = "AIcDu claimers", ToolTip = "The AIcDu to first reach the arena claims all enemies for himself!\n- All enemies will be able to attack once both the player and the director are in the arena,\n- All enemies will only be of one group, ignoring presets.\n- If another director \"walks in\" the arena, all enemies within connect with the new director!"),
	ATM_AICDUrequired		UMETA(DisplayName = "AIcDu required", ToolTip = "The enemies will be able to attack once the concerned director(s) AND the player are in the arena.\n- Group numbers are not altered,\n- In order for the enemies to attack their AIcDu with the same preset-ed group number as them must be in the arena too."),
	ATM_PlayerTriggersAll	UMETA(DisplayName = "Player triggers all", ToolTip = "The enemies are outright aggressive! They re-connect with their directors as soon as the player reaches the arena to attack!\n- Group numbers are not altered,\n- ALL enemies can attack as soon as the player walks in.")
};


UCLASS()
class HOOKNFIGHT_API ANMBV_EnemyZone : public ANavMeshBoundsVolume
{
	GENERATED_BODY()
	
	TArray<class AC_Enemy*> ZonedEnemies;

	TArray<class AAIControlDirectorUnit*> Directors;


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Arena zone", meta = (AllowPrivateAccess = "true"))
	// How the arena will behave in face of the director(s) and/or player entering.
	EEnemyZoneTriggerMode ArenaTriggerMode = EEnemyZoneTriggerMode::ATM_PlayerTriggersAll;


	bool PlayerIn = false;

private:
	// Gets and stores all relevant objects within the volume.
	void GetReleventOverlappingElements();

	const bool ZonedEnemiesDead() const;

protected:
	virtual void BeginPlay() override;

public:
	ANMBV_EnemyZone();


	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
};
