// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIControlDirectorUnit.generated.h"


/*
	The AI Control Director Unit or AICDU is the brain of all control-linkable that have the same group number as him.
	He has for role: 
		- To give guidance to the enemies (Giving attack orders most noticeably)
		- To control spawners, coordinating them for sudden massive spawning (an enemy wave) or keeping feedback whenever they decide to spawn an enemy on their own.

	[Important note]
	This brain in no way, shape or form should interact directly with the enemies.
*/
UCLASS(hideCategories = (Materials, Collision))
class HOOKNFIGHT_API AAIControlDirectorUnit : public AActor
{
	GENERATED_BODY()


	UPROPERTY(NoClear, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	// Used as a root component. Usually not used as an actual collision, but rather for hierarchy organization.
	class USphereComponent* SphereColision;

	UPROPERTY(NoClear, VisibleAnywhere, BlueprintReadOnly, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	// Center glowing sphere. Intangible and all-mighty. Is considered, visually, to be the core brain behind all the enemies.
	class USMC_GlowySphere* MainSphere;

	/* [Feedback]

		An array of randomly rotating cubes.
		They are formed at the beginning of a phase and each represent a wave. The active wave is the white glowing cube. Waves to come are glowing normally. Black cubes are bested waves.
	*/
	TArray<class USMC_GlowyRotatingInclinedCube*> GlowyCubes;

	// Attachment point for the glowy cubes. Solely used for offsetting purposes.
	TArray<class USceneComponent*> GC_AttachPoints;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	// The glowy cubes around the manager rotate locally using offsetting points. This defines how they rotate.
	FRotator OrbitalSpeeds = {-100.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	// The cube themselves get a defined rotation axis as they spawn.
	// This axis is random. The variable here defined how minimally slow it'll rotate around this axis.
	float FragmentLowerRotateSpeedRange = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	// The cube themselves get a defined rotation axis as they spawn.
	// This axis is random. The variable here defined how maximally fast it'll rotate around this axis.
	float FragmentUpperRotateSpeedRange = 60.f;



	// Storage of all connected enemies.
	// Enemies will reach out to the manager as they spawn. More details on the registering function.
	TArray<class UAIRemoteControlPlugin*> EnemyRCPs;

	// Storage of all connected spawners.
	// Spawners will reach out to the manager at the beginning of the game. More details on the registering function.
	TArray<class AA_EnemySpawner*>	EnemySpawners;


	UPROPERTY(NoClear, EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", NoResetToDefault, ToolTip = "How many phases are there, and how many waves of enemy do each have?"))
	// The number of indexes are the number of phases.
	// Each values are the number of waves.
	TArray<int> PhasesAndWaves = {0};

	UPROPERTY(VisibleDefaultsOnly, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	// As long as the manager is able to go on, this will stay on true.
	// The AIcDu Disables as soon as his last wave is over.
	bool Active = true;

	int CurrentPhase = 0;
	bool PhaseActive = false;

	// Global wave number, not affected by the phase system and so never reseted.
	// It is used by the spawners.
	int Global_CurrentWave = 0;

	// Local wave number, reseting each time a new phase starts.
	// Used by the cubes around the AI director.
	int Local_CurrentWave = 0;

	int WavesRemaining;
	bool WaveActive = false;
	bool PreWaveActive = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Feedback", meta = (AllowPrivateAccess = "true"))
	// A lightning effect used when the spawners are triggered.
	class UParticleSystem* LightningParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Feedback", meta = (AllowPrivateAccess = "true"))
	// Lifetime of a lightning launched by the AIcDu.
	float LightningLifeTime = 0.3f;

	// The emitters themselves
	TArray<class UParticleSystemComponent*> Emitters;
	
	// The lifetime of all emitter instantiated by the AIcDu
	TArray<float> EmitterTimers;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", ClampMin = 0.f, UIMin = 1.f, ToolTip = "Time between order toward enemies."))
	float TimeBetweenEnemyOrders = 0.3f;
	float TimerBetweenEnemyOrders;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", ToolTip = "How much time between waves?"))
	float TimeBetweenWaves = 2.f;
	float TimerBetweenWaves;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", ToolTip = "How much time before the next phase begins?"))
	float NextPhaseTime = 5.f;
	float NextPhaseTimer;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", ClampMin = 0, ToolTip = "Limit of melee enemies attacking at the same time."))
	int MeleeAttackerLimit = 3;
	int MeleeAttacking = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Enemy management", meta = (AllowPrivateAccess = "true", ClampMin = 0, ToolTip = "Limit of archer enemies attacking at the same time."))
	int ArcherAttackerLimit = 3;
	int ArcherAttacking = 0;



	UPROPERTY(NoClear, VisibleAnywhere, BlueprintReadOnly, Category = "AICDU Way-points", meta = (AllowPrivateAccess = "true", NoResetToDefault))
	TArray<class ASC_AIcDuWayPoint*> WayPoints;
	
	int CurrentWayPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AICDU|Movability and Arenas", meta = (AllowPrivateAccess = "true", ClampMin = 0.001f, UIMin = 1.f))
	float MovementSpeed = 100.f;

	bool Moving = false;


	int ArenasIn = 0;
	bool PlayerInAnArena = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "AICDU", meta = (AllowPrivateAccess = "true"))
	/* [DEBUG]
	
		Activates a display at runtime:

		Current phase || Current local wave / Phase total waves	|| Currently active enemies  ||
		-------------  > --------------------------------------  > ------------------------   >  Currently attacking enemies
		Total phases  || Current global wave / Total waves		|| Registered enemies		 ||
	*/
	bool DEBUG_AIcDu = false;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AICDU")
	// The group number defines who can link with this director. All elements with a group number and with the same as this director will connect.
	int GroupNumber = 0;

private:
	void CreateNewFragments	(int NumberOfFragments);
	void DestroyOldFragments();

	void OrderAttack();

	void EnemyControlTick	(float DeltaTime);
	void WaveControlTick	(float DeltaTime);
	void PhaseControlTick	(float DeltaTime);

	void EndWave();
	void EndPhase();

	void MovementTick		(float DeltaTime);

	void DirectorShutDown();


	void SpawnLightning(AActor* Target);
	void EmittersLifetimeTick(float DeltaTime);
	
	// Will return true as long as at least one director is active within the world.
	static bool DirectorsActivity(const class UWorld* World);

	void AIcDuDebugDisplay();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	// Signals to the blueprint that a Phase has ended.
	void DirectorBeganPhase();

	UFUNCTION(BlueprintImplementableEvent)
	// Signals to the blueprint that a Phase has ended.
	void DirectorEndedPhase();

	UFUNCTION(BlueprintImplementableEvent)
	// Signals to the blueprint that the AIcDu is now shut down.
	// -> AllDirectorsShutDown: if true, signifies all Directors are shut down.
	void DirectorShuttedDown(bool AllDirectorsShutDown);

public:	
	// Sets default values for this actor's properties
	AAIControlDirectorUnit();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Registers a new Enemy plug.
	// Due to the director not knowing the enemies type, he relies on the plug-ins to give orders and know who is being ordered.
	void RCP_Register(class UAIRemoteControlPlugin* NewEnemyRCP);

	// The plug-out removes an enemy from the guidance of the director. Should only be called by the plug itself.
	// Generally used in case of an Enemy's death.
	void RCP_Plugout(class UAIRemoteControlPlugin* UnpluggedRCP);

	// A simple notification to know what enemy type just successfully completed/stopped an attack cycle.
	void RCP_AttackOverNotification(const TSubclassOf<class AC_Enemy>& EnemyType);

	
	// Registering of an Enemy spawner. Enemy spawners should never be removed once registered.
	void ES_Register(class AA_EnemySpawner* NewEnemySpawner);


	FORCEINLINE void EnteredAnArena()	{ ArenasIn++; }
	FORCEINLINE void LeftAnArena()		{ ArenasIn--; }

	// Is triggered by a player overlap with the arena the AICDU is currently in. Also tells if the director if there was pre-placed enemies to hold off his next wave.
	FORCEINLINE void ArenaPlayerOverlap(bool PrePlacedEnemies) { PlayerInAnArena = true, PreWaveActive = PrePlacedEnemies; }


	UFUNCTION(CallInEditor, Category = "AICDU Way-points")
	// Creates a new way-point for this director and stores it inside the way-point array.
	void NewWayPoint();

	UFUNCTION(CallInEditor, Category = "AICDU Way-points")
	// Deletes the most recently created way-point and removes it from the array.
	void PopWayPoint();

	UFUNCTION(CallInEditor, Category = "AICDU Way-points")
	// Toggle the view of the path that the AIcDu will follow as the phases go on.
	void TogglePath();

	UFUNCTION(CallInEditor, Category = "AICDU Way-points")
	// It is advised to press this button from time to time, it checks if a waypoint broke or one has been accidentally deleted in the editor without using the proper function.
	// If the path is considered "insane", then the odd waypoints will be removed.
	// (One is always performed at BeginPlay but it will be transient.)
	void PathSanityCheck();
};
