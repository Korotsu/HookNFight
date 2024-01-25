// Fill out your copyright notice in the Description page of Project Settings.


#include "SMC_GlowyRotatingInclinedCube.h"

#include "UObject/ConstructorHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"


USMC_GlowyRotatingInclinedCube::USMC_GlowyRotatingInclinedCube()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer"));
	if (CubeMeshAsset.Object) SetStaticMesh(CubeMeshAsset.Object);
}

void USMC_GlowyRotatingInclinedCube::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	AddWorldRotation(Rotation * DeltaTime);
}
