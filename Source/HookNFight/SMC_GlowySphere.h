// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SMC_GlowySphere.generated.h"

/**
 * 
 */
UCLASS(hideCategories = (Materials))
class HOOKNFIGHT_API USMC_GlowySphere : public UStaticMeshComponent
{
	GENERATED_BODY()
	
	// Source used for the material. Remember, it is by default a glowy material with emissive light.
	class UMaterialInstance* GlowyMaterial_InstanceSource;

protected:
	static constexpr float MaterialChangeSpeed = 3.f;
	static constexpr float PositionChangeSpeed = 3.f;
	
	static constexpr FLinearColor	DefaultColor = { 0, 0.5f, 1.f, 1 };
	static constexpr float			DefaultGlow  = 500.f;

	FLinearColor Color = DefaultColor;
	
	FLinearColor TargetColor = DefaultColor;


	float Glowyness = DefaultGlow;

	float TargetGlowyness = DefaultGlow;
	

	FVector TargetLocation;

public:
	USMC_GlowySphere();

public:
	// Used for dynamic material instantiation.
	virtual void OnComponentCreated() override;

	// One awoken, the glowy sphere will only tick as long as it can update. Afterwards it will stop ticking.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Tick")
	// Should be called every frame to smoothly modify the material in runtime.
	// If you only use New[Glow/Color] or Sudden[Glow/Color] you do not need to call this.
	virtual void Update(float DeltaTime);


	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// Sets an immediate glow value on the material. 
	// It will go back to its previous value over time.
	void SuddenGlow(float _ImmediateGlow);

	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// Sets an immediate color value on the material.
	// It will go back to its previous value over time.
	void SuddenColor(const FLinearColor& _ImmediateColor);


	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// The material will get to the desired glow intensity over time.
	void NewGlowTarget(float _GlowTarget);

	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// The material will get to the desired color over time.
	void NewColorTarget(const FLinearColor& _ColorTarget);


	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// Sets a new glow.
	// Immediately changes the current glow, and it will not come back to the previous value.
	void NewGlow(float _NewGlow);

	UFUNCTION(BlueprintCallable, Category = "Color and Glow")
	// Sets a new color.
	// Immediately changes the current color, and it will not come back to the previous value.
	void NewColor(const FLinearColor& _NewColor);


	UFUNCTION(BlueprintCallable, Category = "Movement")
	// Sets up a new relative target for the component. It will smoothly go over time to the desired location.
	void NewTargetLocation(const FVector& _NewTargetLocation);
};