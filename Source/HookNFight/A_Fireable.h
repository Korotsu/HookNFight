// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Fireable.generated.h"

UCLASS(hidedropdown)
class HOOKNFIGHT_API AA_Fireable : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire-able", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMin = 1.f, UIMax = 30.f, ToolTip = "Sets a strict lifetime that counts down as soon as the object is fired.\nThe object will self-destruct as soon as time is up. Set to 0 to disable and give an unlimited lifetime.\n(To set to 0 you have to type it in! The slider was intentionally minimally locked at 1.)"))
	float LifeTime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire-able", meta = (AllowPrivateAccess = "true", ToolTip = "Object self-destructs upon contact with the floor or the walls. (Anything static in the world)"))
	bool StaticContactDestruct = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire-able", meta = (AllowPrivateAccess = "true", UIMin = 0, UIMax = 20, ToolTip = "Damage dealt if contact with the player occurs"))
	int DamagePower = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire-able", meta = (AllowPrivateAccess = "true", ToolTip = "Some meshes are reversed. Turn this on to flip it around."))
	bool Mirror = false;


	bool Fired = false;

	FVector Direction;
	float Speed;

public:	
	// Sets default values for this actor's properties
	AA_Fireable();

	void Fire(const FVector& _Direction, const float _Speed);

	UPROPERTY(NoClear, VisibleDefaultsOnly, BlueprintReadOnly, meta = (NoResetToDefault))
	class UStaticMeshComponent* FiredMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
