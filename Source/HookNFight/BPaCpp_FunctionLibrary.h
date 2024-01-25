// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPaCpp_FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HOOKNFIGHT_API UBPaCpp_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	// Gives the angle formed by two vectors. (Always positive)
	// The vectors are safely normalized and lose a dimension (the specific axis), so you don't need to do it yourself.
	static const float VectorAngle(const FVector& v1, const FVector& v2, const FVector& SpecificAxis = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable)
	// Calls an animation blueprint event from the C++ code.
	static void TriggerAnimBPEvent(class UAnimInstance* AnimInstance, const FString& FunctionName);

	UFUNCTION(BlueprintCallable)
	// Calls a level blueprint event from the C++ code.
	static void TriggerLevelBPEvent(class ULevel* Level, const FString& FunctionName);
};
