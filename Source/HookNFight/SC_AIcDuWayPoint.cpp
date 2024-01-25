// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_AIcDuWayPoint.h"

#include "SMC_GlowySphere.h"

#include "DrawDebugHelpers.h"


// Sets default values
ASC_AIcDuWayPoint::ASC_AIcDuWayPoint()
{
	if (WITH_EDITOR)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
		PrimaryActorTick.bStartWithTickEnabled = false;
	}


	RootTransformComp = CreateDefaultSubobject<USMC_GlowySphere>(TEXT("RootForTransform"));
	RootTransformComp->SetWorldScale3D({0.25f, 0.25f, 0.25f});
	RootTransformComp->NewColor({0, 1.f, 0, 1.f});
	RootTransformComp->bHiddenInGame = true;
	SetRootComponent(RootTransformComp);

}

void ASC_AIcDuWayPoint::Tick(float DeltaTime)
{
	if (PathVisible)
	{
		if (PreviousWayPoint)
		{
			DrawDebugDirectionalArrow(
				GetWorld(),
				PreviousWayPoint->GetActorLocation(),
				GetActorLocation(),
				500.f, FColor::Emerald, false, -1.f, (uint8)'\000', 5.f
			);
		}

		DrawDebugDirectionalArrow(
			GetWorld(),
			GetActorLocation(),
			GetActorLocation() + (TargetLookat.RotateVector(GetActorForwardVector()) * 75.f),
			300.f, FColor::Blue, false, -1.f, (uint8)'\000', 2.f
		);
	}
}

bool ASC_AIcDuWayPoint::ShouldTickIfViewportsOnly() const
{ return true; }

const bool ASC_AIcDuWayPoint::CanMoveOn(int PhasesDone)
{
	HaltForNPhases -= PhasesDone;

	return HaltForNPhases <= 0;
}

void ASC_AIcDuWayPoint::SnapIn(AActor* Director)
{
	Director->SetActorLocationAndRotation( GetActorLocation(), TargetLookat.RotateVector(GetActorForwardVector()).Rotation() );
}

