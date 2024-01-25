// Fill out your copyright notice in the Description page of Project Settings.


#include "SMC_GlowySphere.h"

#include "UObject/ConstructorHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"


USMC_GlowySphere::USMC_GlowySphere()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetComponentTickEnabled(false);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Object) SetStaticMesh(SphereMeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> GlowyMaterialIAsset(TEXT("/Game/h_chemouny/Materials/MI_Glowy"));
	if (GlowyMaterialIAsset.Succeeded()) GlowyMaterial_InstanceSource = GlowyMaterialIAsset.Object;
}

void USMC_GlowySphere::OnComponentCreated()
{
	Super::OnComponentCreated();

	SetMaterial(0, UMaterialInstanceDynamic::Create(GlowyMaterial_InstanceSource, this));

	SetScalarParameterValueOnMaterials("Glowyness", Glowyness);
	SetVectorParameterValueOnMaterials("Coloration", { Color.R, Color.G, Color.B });
}

void USMC_GlowySphere::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Glowyness != TargetGlowyness)
	{
		Glowyness = FMath::Lerp(Glowyness, TargetGlowyness, MaterialChangeSpeed * DeltaTime);

		SetScalarParameterValueOnMaterials("Glowyness", Glowyness);
	}

	if (Color != TargetColor)
	{
		Color = FMath::Lerp(Color, TargetColor, MaterialChangeSpeed * DeltaTime);

		SetVectorParameterValueOnMaterials("Coloration", { Color.R, Color.G, Color.B });
	}


	if ((Glowyness == TargetGlowyness) && (Color == TargetColor)) SetComponentTickEnabled(false);
}

void USMC_GlowySphere::Update(float DeltaTime)
{
	if (Glowyness != TargetGlowyness)
	{
		Glowyness = FMath::Lerp(Glowyness, TargetGlowyness, MaterialChangeSpeed*DeltaTime);

		SetScalarParameterValueOnMaterials("Glowyness", Glowyness);
	}

	if (Color != TargetColor)
	{
		Color = FMath::Lerp(Color, TargetColor, MaterialChangeSpeed*DeltaTime);

		SetVectorParameterValueOnMaterials("Coloration", { Color.R, Color.G, Color.B });
	}

	if (GetRelativeLocation() != TargetLocation)
	{
		SetRelativeLocation( FMath::Lerp(GetRelativeLocation(), TargetLocation, PositionChangeSpeed*DeltaTime) );
	}
}


void USMC_GlowySphere::SuddenGlow(float _ImmediateGlow)
{
	Glowyness = _ImmediateGlow;
	SetScalarParameterValueOnMaterials("Glowyness", _ImmediateGlow);
}

void USMC_GlowySphere::SuddenColor(const FLinearColor& _ImmediateColor)
{
	Color = _ImmediateColor;
	SetVectorParameterValueOnMaterials("Coloration", { _ImmediateColor.R, _ImmediateColor.G, _ImmediateColor.B });
}


void USMC_GlowySphere::NewGlowTarget(float _GlowTarget)
{ TargetGlowyness = _GlowTarget; }

void USMC_GlowySphere::NewColorTarget(const FLinearColor& _ColorTarget)
{ TargetColor = _ColorTarget; }


void USMC_GlowySphere::NewGlow(float _NewGlow)
{
	Glowyness = _NewGlow, TargetGlowyness = _NewGlow;
	SetScalarParameterValueOnMaterials("Glowyness", _NewGlow);
}

void USMC_GlowySphere::NewColor(const FLinearColor& _NewColor)
{
	Color = _NewColor, TargetColor = _NewColor;
	SetVectorParameterValueOnMaterials("Coloration", { _NewColor.R, _NewColor.G, _NewColor.B });
}

void USMC_GlowySphere::NewTargetLocation(const FVector& _NewTargetLocation)
{ TargetLocation = _NewTargetLocation; }
