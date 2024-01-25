// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMC_GlowySphere.h"
#include "SMC_GlowyRotatingInclinedCube.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API USMC_GlowyRotatingInclinedCube : public USMC_GlowySphere
{
	GENERATED_BODY()

public:
	USMC_GlowyRotatingInclinedCube();

	FRotator Rotation = {0, 0, 0};

public:
	void Update(float DeltaTime) override;
};
