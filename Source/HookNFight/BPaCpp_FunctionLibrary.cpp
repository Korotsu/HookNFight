// Fill out your copyright notice in the Description page of Project Settings.


#include "BPaCpp_FunctionLibrary.h"

#include "Animation/AnimInstance.h"
#include "Engine/Level.h"
#include "Misc/OutputDeviceNull.h"

const float UBPaCpp_FunctionLibrary::VectorAngle(const FVector& v1, const FVector& v2, const FVector& SpecificAxis)
{
	if (SpecificAxis == FVector::ZeroVector)
	{
		return FMath::RadiansToDegrees(acosf(FVector::DotProduct(v1.GetSafeNormal(), v2.GetSafeNormal())));
	}
	else if (SpecificAxis == FVector::UpVector)
	{
		return FMath::RadiansToDegrees(acosf(FVector::DotProduct(v1.GetSafeNormal2D(), v2.GetSafeNormal2D())));
	}
	else
	{
		const FVector&& v1_2d = v1 - (SpecificAxis * v1),
						v2_2d = v2 - (SpecificAxis * v2);

		return FMath::RadiansToDegrees(acosf(FVector::DotProduct(v1_2d.GetSafeNormal(), v2_2d.GetSafeNormal())));
	}
}

void UBPaCpp_FunctionLibrary::TriggerAnimBPEvent(UAnimInstance* AnimInstance, const FString& FunctionName)
{
	FOutputDeviceNull ar;
	AnimInstance->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
}

void UBPaCpp_FunctionLibrary::TriggerLevelBPEvent(ULevel* Level, const FString& FunctionName)
{
	FOutputDeviceNull ar;
	Level->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
}
