// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIRemoteControlPlugin.generated.h"


struct FPlugActionInfo
{
	FORCEINLINE FPlugActionInfo(const TSubclassOf<class AC_Enemy>* _TargetType = nullptr, const bool _Success = false)
	: Success(_Success)
	{ TargetType = _TargetType; }

	const TSubclassOf<class AC_Enemy>* TargetType;
	const bool Success;
};

/*
	The Artificial Intelligence Remote Control Plug-in, or shortened to AIRCP is a useful component plugged into any enemy or AI that requires to be coordinated by an external source.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOOKNFIGHT_API UAIRemoteControlPlugin : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Remote Control Plug-in", meta = (AllowPrivateAccess = "true"))
	int GroupNumber = 0;

	class AAIControlDirectorUnit* LinkedDirector;

	TSubclassOf<class AC_Enemy> OwnerType;


	bool PluggedIn = false;

public:	
	// Sets default values for this component's properties
	UAIRemoteControlPlugin();


	FORCEINLINE const int Get_GroupNumber() { return GroupNumber; }

public:
	
	// Sudden destruction must be reported to the AI director!
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Immediate disconnection from the AIcDu (if connected).
	// For the director, it will just seem like an enemy disappeared.
	// (Tip: The group number is NOT affected by this.)
	void Unplug();


	FPlugActionInfo AIP_StartAttack();

	bool AIP_IsAttacking();
	bool AIP_IsHit();

	bool AIP_CanAttack();

	void AIP_NotifyEndAttack();


	FORCEINLINE const TSubclassOf<class AC_Enemy> AIP_GetControlledType()
	{ return OwnerType; }


	/*	
		Attempts to change the linked AIcDu.
		Be warned! In case of a failure the RCP will remain unplugged!

		@return Returns if the re-association worked.
	*/
	bool Replug(const int& NewGroupNumber);

	/*
		Attempt to reconnect to an AIcDu without changing the group number (often leading to the last director the RCP was connected to)
		Be warned! In case of a failure the RCP will remain unplugged!
		
		@return Returns if the re-association worked.
	*/
	bool Replug();


	friend class UEnemyStatusDebug;
};