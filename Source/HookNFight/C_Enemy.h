// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Components/BoxComponent.h"

#include "C_Enemy.generated.h"


UCLASS(hidedropdown, Abstract, hideCategories = (Collision))
class HOOKNFIGHT_API AC_Enemy : public ACharacterBase
{
	GENERATED_BODY()

private:	
	FVector MoveBy = FVector::ZeroVector;

	TArray<AC_Enemy*> NearbyEnemies;


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackChargeTimer;


	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Retreat", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMax = 10.f))
	// If the Player approaches the Enemy, the Enemy will only start retreating after this amount of time.
	float RetreatDelayTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Retreat", meta = (AllowPrivateAccess = "true"))
	// Indicates if the Enemy is able to retreat at all.
	bool CanRetreat = true;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMax = 5.f))
	// Animation and stun time of the Enemy after getting hit.
	float StunTime = 0.666667f;

	// Used for the hit/stun system.
	FTimerHandle StunTimeHandle;

	// Whether the enemy is currently grabbed.
	bool Grabbed = false;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Enemy|AI", meta = (AllowPrivateAccess = "true"))
	// If false, the Enemy will not be able to move or attack.
	// It is automatically set to false at the beginning of the game if the Enemy is placed manually inside an arena zone.
	bool Active = true;

protected:
	UPROPERTY(NoClear, VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UStaticMeshComponent* WeaponMesh;


	UPROPERTY(VisibleAnywhere, Category = "Enemy|AI")
	class UAIRemoteControlPlugin* RemoteControlPlugin;


	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Attack system", meta = (AClampMin = 0.f, ToolTip = "When the Enemy, after a first wait (PreAttackChargeDelay) saw that the player was still reachable, he will charge up his strike for this amount of time."))
	float AttackChargeTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Attack system", meta = (ClampMin = 0.f, ToolTip = "If the Enemy is in the back of the player when he starts preparing his attack, this timer is added."))
	float AttackBackstabDelay = 1.f;


	UPROPERTY(BlueprintReadOnly)
	// This is the attack order solely set to true by the plug-in under an order of the director. Starts a normal attack cycle.
	bool Attacking = false;

	UPROPERTY(BlueprintReadOnly)
	// This variable is unknown to the director and be used to deploy fast attacks at the player without the director actually counting it.
	bool ChargingAttack = false;

public:
	// Sets default values for this character's properties
	AC_Enemy();

	UPROPERTY(EditAnywhere, Category = "Enemy|AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "Enemy|AI|Detection Features")
	class UBoxComponent* CloseZone;

	UPROPERTY(VisibleAnywhere, Category = "Enemy|AI|Detection Features")
	class USphereComponent* PlayerKeepAwayZone;


	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Detection Features", meta = (ClampMin = 0.f, UIMin = 5.f, ToolTip = "The AI has a tolerance around its outer detection zone."))
	float Tolerance = 30.f;


	UPROPERTY(EditAnywhere, Category = "Enemy|AI|Attack system", meta = (ClampMin = 0.f, ToolTip = "After entering the attack cycle, the Enemy will wait this time, leaving the Player an opportunity to escape early."))
	float PreAttackChargeDelay = 1.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (ToolTip = "Can the Enemy be grabbed by the grappling hook ?"))
	bool Grabbable = false;


	FORCEINLINE const bool		Get_CanRetreat()	const { return CanRetreat;						}
	FORCEINLINE const float		Get_RetreatDelay()	const { return RetreatDelayTime;				}
	FORCEINLINE const bool		Is_Active()			const { return Active;							}
	FORCEINLINE const bool		Is_Hit()			const { return hitState;						}

	// Get the instruction given by the director. May differ from what the enemy is actually doing...
	FORCEINLINE const bool		Get_AttackOrder()	const { return Attacking;						}

	// Get the actual attack status of the enemy.
	FORCEINLINE const bool		Is_Attacking()		const { return (Attacking || ChargingAttack);	}


	// Changes the value of grabbed.
	// When set to true, the enemy will also stop moving immediately, and will not be in capacity to move until he's un-grabbed
	void Set_Grabbed(const bool Value);


	// A useful function able to give back a normalized, and then scaled up value of the MoveBy vector. It will be scaled up in function of the maximum speed of the character to avoid step-by-step movements.
	// Warning: Calling this also empties MoveBy as to prevent multi-usage.
	const FVector SafeScale_MoveBy();

private:
	UFUNCTION(meta = (AllowPrivateAccess = "true"))
	// Automatically called after the stun timer handle is over.
	void EndHit();

protected:
	void PlayNoise(USoundBase* Sound);

public:
	
	/*
		"Turns on" the Enemy, allowing to move and do proximity attacks.
		The enemy's RCP will need to be plugged in to allow the Enemy to attack normally.
		(Note that nothing will happen if the enemy is dead.)

		@param Replug Allow to automatically reconnect the RCP with the previous director.
	*/
	void Activate(bool Replug = false);

	/*
		More impacting version of "Activate", that takes a new group number for the RCP immediately.
		"Turns on" the Enemy and connects him with a director. Allowing him both to move and attack normally.
		(Note that nothing will happen and false will be returned if the enemy is dead.)

		@param NewGroupNumber The target director to connect with.
		@return The Success of the re-plugging.
	*/
	bool Activate(const int& NewGroupNumber);
	
	/*
		"Turns off" the Enemy, preventing him to move or attack.
		(Note that nothing will happen if the enemy is dead.)

		@param Unplug additional instruction to unplug the enemy's RCP. (Will prevent the Enemy from attacking until he's re-associated with an AICDU.)
	*/
	void Deactivate(bool Unplug = false);

	/*
		Replug the RCP, without changing the group number or the activity, if the sent group number matches.
		
		Often results in re-connecting silently with the previous/preset director.
		Only works if the Enemy's RCP was unplugged.
		(Note that nothing will happen if the enemy is dead.)
	*/
	void Reconnect(const int& TestGroupNumber);

	/*
		Checks the current group number of the Enemy's RCP using the given value.
		If a discrepancy is found and the RCP is allowed to, the plugin will re-associate itself with the proper AIcDu.

		@param DesiredGroupNumber The desired target GroupNumber.
		@param ReplugAuthorized Allows the RCP to re-associate with another AIcDu (Note that nothing will happen if the enemy is dead).

		@return Whether the GroupNumbers were matching, or not.
	*/
	const bool EnsureGroupNumber(const int& DesiredGroupNumber, const bool&& ReplugAuthorized = true);


	// Executes the accumulated movement value MoveBy using a smooth AI control (MoveTo), emptying the MoveBy vector back to {0, 0, 0}.
	// Thanks to MoveBy usually being a short vector, the enemy won't be stuck in the MoveTo for a while, allowing repetitive/chained usage.
	void ExecuteMovement();


	// Begins the Attack cycle of the Enemy.
	void BeginAttack() {Attacking = true;}
	
	// Checks that the Player is still reachable.
	// If it's a success, returns true and sets ChargingAttack on true as well to continue the attack cycle.
	virtual bool BeginAttackCharge() {return false;}

	// Strikes down, dealing damage if the player is still in range. Returns false otherwise.
	virtual bool ExecuteAttack() {return false;}


	// Calculates, returns, and stores the charge time of the attack accordingly to the enemy potentially being behind the player.
	float CalculateChargeTime(const FVector& PlayerPos, const FVector& PlayerForward);


	// Deals immediate damage to the Enemy, messing with his behavior as well.
	void Get_Hit(float Damage) override final;

	// Ends the attack cycle and sends a report to the plug, relayed to the Director.
	void AbortAttackCycle();


	// Detects all enemies inside the CloseZone and stores them into NearbyEnemies.
	void DetectNearbyEnemies();

	// Detects elements of your choice inside the CloseZone, giving them back into an array.
	template<class T>
	FORCEINLINE TArray<T*> DetectNearby()
	{
		TArray<AActor*> NearbyActors;

		CloseZone->GetOverlappingActors(NearbyActors, TSubclassOf<T>());


		TArray<T*> AskedEs;

		for (AActor* A : NearbyActors)
		{
			T* AnAskedE = Cast<T>(A);

			if (AskedE) AskedEs.Add(AnAskedE);
		}

		 
		return AskedEs;
	}

	// Gives back the first Element of your choice detected inside the close zone.
	template<class T> 
	FORCEINLINE T* DetectUnique()
	{
		TArray<AActor*> NearbyActors;

		CloseZone->GetOverlappingActors(NearbyActors, TSubclassOf<T>());

		for (AActor* A : NearbyActors)
		{
			T* AskedE = Cast<T>(A);

			if (AskedE) return AskedE;
		}

		return nullptr;
	}

	// Adds movement that can be executed later (The MoveBy vector is not editable externally.)
	FORCEINLINE void AddMovement(const FVector& AdditionalMoveBy) {MoveBy +=  AdditionalMoveBy;}

	// Do not forget to update this with DetectNearbyEnemies() first !
	FORCEINLINE const TArray<AC_Enemy*>& GetNearbyEnemies() {return NearbyEnemies;}


	FORCEINLINE FVector	GetRightUnit2DVector  () const { return GetRootComponent()->GetRightVector()  .GetSafeNormal2D(); };
	FORCEINLINE FVector	GetForwardUnit2DVector() const { return GetRootComponent()->GetForwardVector().GetSafeNormal2D(); };


	friend class UEnemyStatusDebug;
};
