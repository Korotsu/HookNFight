// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Fireable.h"

#include "HookNFightCharacter.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AA_Fireable::AA_Fireable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FiredMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	FiredMesh->SetCollisionProfileName("Fireable");
	SetRootComponent(FiredMesh);
}

void AA_Fireable::Fire(const FVector& _Direction, const float _Speed)
{
	Direction = _Direction.GetSafeNormal(), Speed = _Speed;
	Fired = true;

	SetActorRotation(Mirror ? (-Direction).Rotation() : Direction.Rotation());
}

// Called when the game starts or when spawned
void AA_Fireable::BeginPlay()
{
	Super::BeginPlay();
	
	FiredMesh->OnComponentBeginOverlap.AddDynamic(this, &AA_Fireable::BeginOverlap);
	FiredMesh->OnComponentHit.AddDynamic(this, &AA_Fireable::OnCompHit);
}

// Called every frame
void AA_Fireable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Fired)
	{
		if		(LifeTime > 0) LifeTime -= DeltaTime;
		else if (LifeTime < 0) Destroy();


		AddActorWorldOffset(Direction * Speed * DeltaTime);
	}
}

void AA_Fireable::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic || 
		OtherActor->ActorHasTag(TEXT("Floor")) || OtherActor->ActorHasTag(TEXT("Wall")))
	{ Destroy(); }

	AHookNFightCharacter* Player = Cast<AHookNFightCharacter>(OtherActor);
	if (Player)
	{
		Player->Get_Hit(DamagePower);
		Destroy();
	}
}

void AA_Fireable::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{ if (StaticContactDestruct) Destroy(); }
