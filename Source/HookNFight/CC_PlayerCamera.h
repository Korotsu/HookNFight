// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "HookNFightCharacter.h"

#include "CC_PlayerCamera.generated.h"


UENUM(BlueprintType)
enum class ECameraMovementRelativity : uint8
{
	CMR_AlwaysRelative		UMETA(DisplayName = "Always relative"		, ToolTip = "The movement forward of the player always where the camera is looking towards."),
	CMR_PartiallyRelative	UMETA(DisplayName = "Partially relative"	, ToolTip = "The movement forward of the player is where the camera was last looking towards until the player started moving.")
};

UENUM(BlueprintType)
enum class ECameraResetMode : uint8
{
	CRM_Instantaneous		UMETA(DisplayName = "Instant reset"			, ToolTip = "The Camera comes back behind the player instantly upon reset."),
	CRM_Smooth				UMETA(DisplayName = "Smooth reset"			, ToolTip = "The Camera comes back smoothly behind the player as long as the reset input is pressed.")
};

UENUM(BlueprintType)
enum class ECameraShakeAddupMode : uint8
{
	// The shake values simply add up normally.
	CSAM_AddUp				UMETA(DisplayName = "Normal addition"),

	// The shake values add up but incoming values above "Shake intensity roof" are divided by 2.
	CSAM_AddRestrained		UMETA(DisplayName = "Restrained addition"),

	// The shake values add up but the result cannot above the "Shake intensity roof".
	CSAM_AddLimited			UMETA(DisplayName = "Limited addition"),

	// The incoming value becomes the current shake value.
	CSAM_Equal				UMETA(DisplayName = "Equal")
};

UENUM(BlueprintType)
enum class ECameraShakeIntensity : uint8
{	
	// A strong camera shake intensity, of 100 units around at it's beginning.
	CSI_Powerful	UMETA(DisplayName = "Hard shake"),
	
	// A medium camera shake intensity, of 60 units around at it's beginning.
	CSI_Medium		UMETA(DisplayName = "Medium shake"),
	
	// A light camera shake intensity, of 25 units around at it's beginning.
	CSI_Light		UMETA(DisplayName = "Light shake")
};

enum MovementDirection
{MD_Forward, MD_Right, MD_Num};


UCLASS()
class HOOKNFIGHT_API UCC_PlayerCamera : public UCameraComponent
{
	GENERATED_BODY()	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Move relativity", meta = (AllowPrivateAccess = "true"))
	// Player-Camera-Movement relativity mode.
	ECameraMovementRelativity MoveRelativity = ECameraMovementRelativity::CMR_PartiallyRelative;
	
	FRotationMatrix CurrentMoveLandmark = FRotationMatrix(FRotator::ZeroRotator);
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Pitch control", meta = (AllowPrivateAccess = "true", ClampMin = -90.f, ClampMax = 0.f))
	// Minimal downward rotation allowed by the camera boom.
	float MinCamPitch = -30.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Pitch control", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, ClampMax = 90.f))
	// Maximal upward rotation allowed by the camera boom.
	float MaxCamPitch = 80.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Shake", meta = (AllowPrivateAccess = "true"))
	// How the camera will handle multiple incoming shakes that need to add up
	ECameraShakeAddupMode ShakeAddupMode = ECameraShakeAddupMode::CSAM_AddRestrained;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Shake", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMin = 100.f))
	// (See "ShakeAddupMode" for more info on what this does as a limitation)
	float ShakeIntensityRoof = 125.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Shake", meta = (AllowPrivateAccess = "true", ClampMin = 0.1f))
	// Controls how fast the camera shakes go back down entirely. (in units per second)
	float ShakeCooldown = 150.f;

	float CurrentShakeIntensity = 0.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Reset", meta = (AllowPrivateAccess = "true"))
	ECameraResetMode ResetMode = ECameraResetMode::CRM_Smooth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Camera Management|Reset", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMin = 0.0001f))
	float ResetSpeed = 3.f;

	bool Reseting = false;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Custom Camera Management", meta = (AllowPrivateAccess = "true"))
	// Allows to see the Landmark currently used.
	// Blue if currently free, and red if locked.
	bool DEBUG_MovementLandmarkView = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Custom Camera Management", meta = (AllowPrivateAccess = "true"))
	// Allows to see the camera's pitch and yaw in live (in blue), as well as the Player's controller rotation values (in red)!
	bool DEBUG_ControllerRotationView = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Custom Camera Management", meta = (AllowPrivateAccess = "true"))
	/*	See how much the camera is shaking with a unique display!
		
		=== Camera shake ===
		[-----------------------||---->]
		<ShakeValue>

		The arrow represents how much the camera is shaking.
		- Red if equal or over a strong shake,
		- orange if over a medium shake, but less than a strong shake,
		- green-yellow is under a medium shake, but over a light one,
		- blue if under a light shake.

		The "||" appears if the shake is over the roof, as the arrow pushes the "]" further!
	*/
	bool DEBUG_ShakeIntensityView = false;


	AHookNFightCharacter* Player = nullptr;

	float	CurrentYawOffset = 0.f,
			CurrentPitchOffset = 0.f;

public:
	static constexpr float StrongShakeIntensity = 100.f;
	static constexpr float MediumShakeIntensity = 60.f;
	static constexpr float LightShakeIntensity  = 30.f;

private:
	inline void AddShakeIntensity(const float& AdditionalIntensity)
	{
		switch (ShakeAddupMode)
		{
		case ECameraShakeAddupMode::CSAM_AddUp:
			CurrentShakeIntensity += AdditionalIntensity;
			return;

		case ECameraShakeAddupMode::CSAM_AddRestrained:
			if		(CurrentShakeIntensity + AdditionalIntensity	>= ShakeIntensityRoof)   CurrentShakeIntensity += AdditionalIntensity/2.f;
			else  /*(CurrentShakeIntensity + AdditionalIntensity	<  ShakeIntensityRoof)*/ CurrentShakeIntensity += AdditionalIntensity;
			
			return;

		case ECameraShakeAddupMode::CSAM_AddLimited:
			if		(CurrentShakeIntensity							>= ShakeIntensityRoof)   return;
			else if (CurrentShakeIntensity + AdditionalIntensity	>= ShakeIntensityRoof)   CurrentShakeIntensity  = ShakeIntensityRoof;
			else  /*(CurrentShakeIntensity + AdditionalIntensity	<  ShakeIntensityRoof)*/ CurrentShakeIntensity += AdditionalIntensity;

			return;

		case ECameraShakeAddupMode::CSAM_Equal:
			CurrentShakeIntensity = AdditionalIntensity;
			return;
		}
	}

public:
	// Only once.
	// @param CameraBoom, the tether from player to this camera,
	// @param PlayerController, the controller of the player.
	FORCEINLINE void Init(class AHookNFightCharacter* _Player)
	{
		Player = _Player;

		if (GetRelativeLocation().IsNearlyZero() == false)
		{
			Player->Controller->SetControlRotation((Player->GetActorLocation() - GetComponentLocation()).Rotation());
			SetRelativeRotation(FRotator::ZeroRotator);
		}
	}

	void CameraUpdate(float DeltaTime);

	// As long as ForwardDirection is not locked, it calculates a new one and returns it. 
	// Otherwise, all calculations are skipped and ForwardDirection is returned immediately.
	const FRotationMatrix& Get_CurrentMoveLandmark();

	FORCEINLINE void ControllerYawAndPitch(float& Yaw, float& Pitch)
	{
		const FVector&& PlayerToCam		= GetComponentLocation() - Player->GetActorLocation(),
						PlayerForwardV	= -Player->GetActorForwardVector().GetSafeNormal2D();

		Yaw
		= FQuat::FindBetweenVectors(PlayerForwardV, PlayerToCam).Rotator().Yaw;

		const FVector&& GroundedPlayerToCam = PlayerForwardV.RotateAngleAxis(Yaw, { 0, 0, 1 });

		Pitch
		= FMath::RadiansToDegrees(FQuat::FindBetweenVectors(GroundedPlayerToCam, PlayerToCam).GetAngle())
		* (GetComponentLocation().Z > Player->GetActorLocation().Z ? 1.f : -1.f);
	}

	FORCEINLINE const float ControllerPitchLimiter(const float DesiredPitchOffset)
	{
		if		((DesiredPitchOffset >0) && DesiredPitchOffset + CurrentPitchOffset >= MaxCamPitch)
		{
			if (CurrentPitchOffset < MaxCamPitch)
				return MaxCamPitch - CurrentPitchOffset;
			else
				return 0.f;
		}
		else if ((DesiredPitchOffset <0) && DesiredPitchOffset + CurrentPitchOffset <= MinCamPitch)
		{
			if (CurrentPitchOffset > MinCamPitch)
				return MinCamPitch - CurrentPitchOffset;
			else
				return 0.f;
		}
		else return DesiredPitchOffset;
	}

	FORCEINLINE void ShakeCamera(const float Intensity)
	{ AddShakeIntensity(Intensity); }

	FORCEINLINE void ShakeCamera(const ECameraShakeIntensity Intensity)
	{
		switch (Intensity)
		{
		case ECameraShakeIntensity::CSI_Powerful:
			AddShakeIntensity(StrongShakeIntensity);
			break;
		
		case ECameraShakeIntensity::CSI_Medium:
			AddShakeIntensity(MediumShakeIntensity);
			break;
		
		case ECameraShakeIntensity::CSI_Light:
			AddShakeIntensity(LightShakeIntensity);
			break;
		}
	}
	

	FORCEINLINE void CameraResetStart() {Reseting = true;}


	bool DirectionInputs[MovementDirection::MD_Num] = {false, false};
	FORCEINLINE void PlayerMoved(const bool&& _Value, const MovementDirection&& _Direction)
	{ DirectionInputs[_Direction] = _Value; }
};
