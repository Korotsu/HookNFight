// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HookNFightCharacter.generated.h"

UCLASS()
class HOOKNFIGHT_API AHookNFightCharacter : public ACharacterBase
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		/** Camera boom positioning the camera behind the character. */
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		/** Follow camera. */
		class UCC_PlayerCamera* FollowCamera;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = 0.f))
		float cameraRange = 500;

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
		/** The current combo number of the player. */
		int comboCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (UIMin = 0))
		/** The maximum +1 combo the player can reach. */
		int comboMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (UIMin = 0.f))
		/** The delay let to the player to start an attack again before reseting the combo number. */
		float attackOffsetToStopCombo = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo", meta = (UIMin = 0.f))
		/** The minimum amout of time the player have to wait until he can attack again. */
		float timeOffsetBetweenTwoAttack;

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
		/** Litterally the time since the last attack. */
		float timeSinceLastAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** Draw ingame the Debug Lines for target system. */
		bool drawDebugLineTargetSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** The debug scale sphere speed.*/
		float DEBUGScaleSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** The debug scale sphere speed.*/
		float DEBUGScaleMax = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** Make the Debug lines permanent. */
		bool permanentLineTargetSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** autoHit. */
		bool DEBUGautoHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		/** Lock the movement while attacking . */
		bool moveLockWhileAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grapple")
		/** The mesh for the base of the grapple.*/
		UStaticMeshComponent* baseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple", meta = (UIMin = 0.f))
		/** The Grapple range min. */
		float grappleRangeMin = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple", meta = (UIMin = 0.f))
		/** The maximum range of the target system and grapple. */
		float targetRange = 2750;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple", meta = (UIMin = 1.f))
		/** The Grapple projection speed. */
		float grappleSpeed = 65;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple", meta = (UIMin = 1.f))
		/** The Grapple towing speed. */
		float grappleTowSpeed = 35;

	UPROPERTY(BlueprintReadWrite, Category = "Grapple")
		/** The current Target of the player. */
		AActor* target;

	UPROPERTY(BlueprintReadWrite, Category = "State")
		/** The attack status.*/
		bool attack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		/** The maximum life of the player. */
		float maxLife = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (UIMin = 0.f))
		/** The attack Speed :) .*/
		float attackSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (UIMin = 0.f))
		/** How many time you stay immune after the hit annimation. */
		float immuneTimeAfterGettingHit = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
		/** The fixed time register after the hit animation is completed. */
		float timeAfterHitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Attack")
		/** The maximum amount of the gauge.*/
		float gaugeSPMaxAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Attack")
		/** The SP gained after succesfully completed an attack.*/
		float _SPgainedAfterAttackSucces;


	UPROPERTY(BlueprintReadWrite, Category = "Super Attack")
		/** The SP gained after succesfully completed an attack.*/
		float currentSPAmount;

	UPROPERTY(BlueprintReadWrite, Category = "Super Attack")
		/** The SP gained after succesfully completed an attack.*/
		bool superAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Attack")
		/** The debug scale sphere speed.*/
		float sp_ScaleSpeed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Attack")
		/** The debug scale sphere speed.*/
		float sp_ScaleMax = 100;

	/** Disable input state. */
	bool disableInput2;

	/** disable input state but you can continue to move. */
	bool disableInputButMovement;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
		/** The hitbox of the 1st combo attack.*/
		class UStaticMeshComponent* h_Combo1;

	UPROPERTY(EditAnywhere, Category = "Combo", meta = (UIMin = 0.f))
		/** The damage of the 1st combo attack.*/
		float h_Combo_1_Damage = 1.f;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
		/** The hitbox of the 2nd combo attack.*/
		class UStaticMeshComponent* h_Combo2;

	UPROPERTY(EditAnywhere, Category = "Combo", meta = (UIMin = 0.f))
		/** The damage of the 2nd combo attack.*/
		float h_Combo_2_Damage = 1.f;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
		/** The hitbox of the 3rd combo attack.*/
		class UStaticMeshComponent* h_Combo3;

	UPROPERTY(EditAnywhere, Category = "Combo", meta = (UIMin = 0.f))
		/** The damage of the 3rd combo attack.*/
		float h_Combo_3_Damage = 1.f;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
		/** The hitbox of the 4th combo attack.*/
		class UStaticMeshComponent* h_Combo4;

	UPROPERTY(EditAnywhere, Category = "Hitbox")
		/** The hitbox of the 4th combo attack.*/
		class UStaticMeshComponent* h_SP_Hitbox;

	UPROPERTY(EditAnywhere, Category = "Combo", meta = (UIMin = 0.f))
		/** The damage of the 4th combo attack.*/
		float h_Combo_4_Damage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		/** The mesh for the base of the grapple.*/
		class UBoxComponent* dodgeDetectionHitbox;

	UPROPERTY(EditAnywhere, Category = "Combo")
		/** Just a sorting tab.*/
		class USceneComponent* hitboxTab;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	/** The parameter for the inversion of X axis.*/
	bool XAxisInverted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	/** The parameter for the inversion of Y axis.*/
	bool YAxisInverted = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		/** The sound when Successfully hit.*/						   
		USoundBase* hitSuccessfullSound;					   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		/** The sound when missing a hit.*/						   
		USoundBase* hitMissSound;							   
														   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		/** The sound when using the special attack.*/
		USoundBase* specialAttackSound;

	/** Just a timestamp after the fire of hitSuccessfullSound sound.*/
	float timeAfterLastHitSuccessfullSound;

	/** Just a timestamp after the fire of hitMissSound sound.*/
	float timeAfterLastHitMissSound;

	/** Just a timestamp after the fire of specialAttackSound sound.*/
	float timeAfterLastSpecialAttackSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreenShakes")
	// How much is the screen shaken when the player is hit?
	float DamageShake = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScreenShakes", meta = (ClampMin = 150.f))
	// Continuous screen shake for as long as the special attack is active.
	float SpecialShake = 200.f;

public:

	/** The Boolean to check if the player already launch the grapple.*/
	bool alreadyLaunchedGrapple;

	/** The Grapple Mesh.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Grapple")
		UStaticMeshComponent* grappleMesh;
	
	/** The security offset of the grapple when he is close to the target.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grapple", meta = (UIMin = 0.f))
		float grabRangeOffSet = 5;

	/** The towed state of the player.*/
	UPROPERTY(BlueprintReadWrite, Category = "Grapple")
		bool towed;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
		/** The spawn point.*/
		FVector spawnPoint;

	UPROPERTY(BlueprintReadWrite, Category = "State")
		/** The immune status.*/
		bool immune;

private :

protected:

	/////////////////Startup Functions/////////////////////
	virtual void BeginPlay() override;

	//Function to setup Player Input Functions;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/////////////////Startup Functions/////////////////////


	/////////////////Update Functions//////////////////////

	//Disable Input Function (check if disable input flag is on then disable inputs);
	void DisableInput2();

	//Update the current target;
	void CheckForTarget();

	//Check the current state of the combo system;
	void Check_Attack();

	//Run the Super Attack;
	void SuperAttack(float deltatime);

	//Custom  hitbox attack detection;
	void Do_Attack(UStaticMeshComponent* hitbox, float timeBeforeActivation, float damage);

	/////////////////Update Functions//////////////////////

	/////////////////Fired Functions///////////////////////

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Hit)
		void TargetHighlight(AActor* target2, bool state);

	//Player Input Functions;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void MouseTurnAtRate(float Rate);
	void MouseLookUpAtRate(float Rate);
	void FireGrapple();
	void Attack();
	void LaunchSuperAttack();
	void Settings();
	//void Spawn_Hitbox();

	void DEBUGGet_Hit();
	void DEBUGScaleSphere(float scale);
	void DEBUGRespawn();

	/////////////////Fired Functions///////////////////////

public:

	/** The constructor.*/
	AHookNFightCharacter();

	/** The attack fire after the end of the grapple on an enemy.*/
	void GrappleAttack();

	void Get_Hit(float damage);

	void Regen();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Hit)
		void ImmuneHighlight(bool state);

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/

	FORCEINLINE class UCC_PlayerCamera* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AActor* GetTarget() const
	{
		return target;
	}

	FORCEINLINE void SetTarget(AActor * _target)
	{
		if (target)
		{
			TargetHighlight(target, false);

			if (_target)
			{
				TargetHighlight(_target, true);
			}
		}

		else
		{
			if (_target)
			{
				TargetHighlight(_target, true);
			}
		}

		target = _target;
	}
};

