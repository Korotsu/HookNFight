// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_PlayerCamera.h"

#include "HookNFightCharacter.h"
#include "BPaCpp_FunctionLibrary.h"

#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "I_DebugMesssagesManager.h"
#include "DrawDebugHelpers.h"


void UCC_PlayerCamera::CameraUpdate(float DeltaTime)
{
	ControllerYawAndPitch(CurrentYawOffset, CurrentPitchOffset);
	
	if (Reseting) switch (ResetMode)
	{
	case (ECameraResetMode::CRM_Smooth):
	{
		float YawReducer = -CurrentYawOffset * ResetSpeed * DeltaTime;
			
		Player->AddControllerYawInput(YawReducer);

		CurrentYawOffset -= (CurrentYawOffset > 0 ? YawReducer : -YawReducer);
		if (FMath::IsNearlyZero(CurrentYawOffset, 1.f)) Reseting = false;
			
		break;
	}

	case (ECameraResetMode::CRM_Instantaneous):
		Player->AddControllerYawInput(-CurrentYawOffset);
		Reseting = false;
		break;
	}

	if (CurrentShakeIntensity > 0)
	{
		FVector&& Shake = FVector(0, 0, CurrentShakeIntensity);

		Shake = FRotator(0.f, 0.f, FMath::FRandRange(0.f, 360.f)).RotateVector(Shake);

		SetRelativeLocation(Shake);

		CurrentShakeIntensity -= ShakeCooldown * DeltaTime;
	}
	else if (GetRelativeLocation().IsZero() == false || CurrentShakeIntensity != 0)
	{
		SetRelativeLocation(FVector::ZeroVector);
		CurrentShakeIntensity = 0;
	}

	// =====[DEBUG]===== //
	{
		if (DEBUG_MovementLandmarkView)
		{
			Get_CurrentMoveLandmark();

			DrawDebugDirectionalArrow(
				GetWorld(),
				Player->GetActorLocation() - CurrentMoveLandmark.GetUnitAxis(EAxis::X) * 200.f,
				Player->GetActorLocation() + CurrentMoveLandmark.GetUnitAxis(EAxis::X) * 200.f,
				200.f,

				(MoveRelativity == ECameraMovementRelativity::CMR_AlwaysRelative ? FColor::Green : 
				 (DirectionInputs[0] == false) && (DirectionInputs[1] == false)  ? FColor::Blue  : 
																				   FColor::Red   ),
				false, -1.f, (uint8)'\000', 7.f
			);

			DrawDebugLine(
				GetWorld(),
				Player->GetActorLocation() - CurrentMoveLandmark.GetUnitAxis(EAxis::Y) * 200.f,
				Player->GetActorLocation() + CurrentMoveLandmark.GetUnitAxis(EAxis::Y) * 200.f,

				(MoveRelativity == ECameraMovementRelativity::CMR_AlwaysRelative ? FColor::Green : 
				 (DirectionInputs[0] == false) && (DirectionInputs[1] == false)  ? FColor::Blue  : 
																				   FColor::Red   ),
				false, -1.f, (uint8)'\000', 7.f
			);
		}

		if (DEBUG_ControllerRotationView)
		{
			GetDMM(GetWorld())->AddMessage(
			FDebugMessage(FColor::Cyan, FString::Printf(TEXT("=== Camera ===\nYaw: %f\nPitch: %f"), CurrentYawOffset, CurrentPitchOffset))
			);

			GetDMM(GetWorld())->AddMessage(
			FDebugMessage(FColor::Red, FString("=== Controller ===\n") + Player->Controller->GetControlRotation().ToString())
			);
		}

		AddShakeIntensity(0.25f);

		if (DEBUG_ShakeIntensityView)
		{
			FColor ArrowColor;

			if		(CurrentShakeIntensity >= StrongShakeIntensity) ArrowColor = FColor::Red;
			else if (CurrentShakeIntensity >= MediumShakeIntensity) ArrowColor = FColor::Orange;
			else if (CurrentShakeIntensity >= LightShakeIntensity)  ArrowColor = FLinearColor(0.5f, 1.f, 0.f).ToFColor(false);
			else													ArrowColor = FColor::Cyan;


			int DebugShakeValue = ((int)CurrentShakeIntensity) / 2;
			int DebugShakeRoof = ((int)ShakeIntensityRoof) / 2;
			FString ShakeView = "=== Camera shake ===\n[";

			if		(DebugShakeValue < DebugShakeRoof)
			{ 
				for (int i = 0; i < DebugShakeValue; i++ ) ShakeView += '-'; 
				if (DebugShakeValue >0) ShakeView += '>'; 
			
				for (int i = 0; i < DebugShakeRoof - DebugShakeValue; i++) ShakeView += "  ";
				ShakeView += ']';
			}
			else if (DebugShakeValue == DebugShakeRoof)
			{
				for (int i = 0; i < DebugShakeValue; i++) ShakeView += '-';
				ShakeView += '>';

				ShakeView += ']';
			}
			else if (DebugShakeValue > DebugShakeRoof)
			{
				for (int i = 0; i < DebugShakeRoof; i++) ShakeView += '-';
				ShakeView += "||";

				if (DebugShakeValue >= DebugShakeRoof +1)
					for (int i = 0; i < DebugShakeValue - (DebugShakeRoof + 2); i++) ShakeView += '-';
					ShakeView += '>';
				
				ShakeView += ']';
			}

			ShakeView += FString::Printf(TEXT("\nShake value: %f"), CurrentShakeIntensity);

			GetDMM(GetWorld())->AddMessage( FDebugMessage(ArrowColor, ShakeView) );
		}
	}
}

const FRotationMatrix& UCC_PlayerCamera::Get_CurrentMoveLandmark()
{
	if (((DirectionInputs[0] == false) && (DirectionInputs[1] == false)) || MoveRelativity == ECameraMovementRelativity::CMR_AlwaysRelative)
	{
		// find out which way is right
		const FRotator&& Rotation = Player->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Make the MoveLandmark
		CurrentMoveLandmark = FRotationMatrix(YawRotation);
	}
	
	return CurrentMoveLandmark;
}