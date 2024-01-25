// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookNFightCharacter.h"
#include "A_Grapple.generated.h"

UCLASS()
class HOOKNFIGHT_API AA_Grapple : public AActor
{
	GENERATED_BODY()

protected:

	/** The grapple base position.*/
	FVector base;

	/** The target reference.*/
	AActor* target;

	/** The forward state.*/
	bool goForward;

	/** The backward state.*/
	bool goBackward;

	/** The parent reference.*/
	AHookNFightCharacter* parent;

	/** The towing state.*/
	bool grabPlayer;

	/** The rope of the grappling hook.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cable, meta = (AllowPrivateAccess = "true"))
		class UCableComponent* rope;

	/** The sound when towing.*/
	USoundBase* towingSound;

	/** The sound when moving.*/
	USoundBase* grappleSound;

	/** The sound when you catch the target.*/
	USoundBase* catchSound;

	/** The sound player component.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cable, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* soundPlayer;

	/** Just a timestamp after the fire of towing sound.*/
	float timeAfterLastTowingSound;

	/** Just a timestamp after the fire of Grapple sound.*/
	float timeAfterLastGrappleSound;

	/** A little offset to secure the grab detection.*/
	float grabRangeOffSet;

	/** The speed of the grapple.*/
	float speed;

	/** The towing speed of the grapple.*/
	float towingSpeed;

	float currentVolume = 0;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** The constructor of the grapple.*/
	AA_Grapple();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** The static function to instantiate a grapple.*/
	static AA_Grapple* Launch(AActor* target, float speed, float towSpeed, UWorld* world, AHookNFightCharacter* parent);

};
