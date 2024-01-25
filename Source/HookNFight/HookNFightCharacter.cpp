// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "HookNFightCharacter.h"

#include "A_Grapple.h"
#include "CC_PlayerCamera.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "A_Fireable.h"

#include "UObject/ConstructorHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"


// Debug include
#include "C_Enemy.h"

//////////////////////////////////////////////////////////////////////////
// AHookNFightCharacter

AHookNFightCharacter::AHookNFightCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCC_PlayerCamera>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	alreadyLaunchedGrapple = false;

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("baseMesh"));
	baseMesh->SetupAttachment(GetMesh(), "LeftWeaponShield");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseVisualAsset(TEXT("/Game/w_nardone/Game/Grapple/Meshes/Grapple"));
	if (baseVisualAsset.Succeeded())
	{
		baseMesh->SetStaticMesh(baseVisualAsset.Object);
		UE_LOG(LogTemp, Warning, TEXT("MeshConstructed ! 2525"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshConstruction failed!"));
	}

	baseMesh->SetRelativeLocationAndRotation({ 4.500000,0.000000,-4.200000 }, { 110.000000,1440.000000,1440.000000 });
	baseMesh->SetWorldScale3D({0.030000,0.082500,0.082500 });

	grappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("grappleMesh"));
	grappleMesh->SetupAttachment(baseMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> grappleVisualAsset(TEXT("/Game/w_nardone/Game/Grapple/Meshes/Grapple"));
	if (grappleVisualAsset.Succeeded())
	{
		grappleMesh->SetStaticMesh(grappleVisualAsset.Object);
		UE_LOG(LogTemp, Warning, TEXT("MeshConstructed ! 3535"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshConstruction failed!"));
	}

	grappleMesh->SetRelativeLocationAndRotation({-166.600006,0.000000,34.000000},{0.000000,180.000000,-180.000000});
	grappleMesh->SetWorldScale3D({33.000000,12.000000,12.000000});

	attack		= false;
	comboCount	= 1;

	hitboxTab = CreateDefaultSubobject<USceneComponent>(TEXT("HitboxTab"));
	hitboxTab->SetupAttachment(RootComponent);

	h_Combo1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("H_Combo1"));
	h_Combo1->SetupAttachment(hitboxTab);

	h_Combo2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("H_Combo2"));
	h_Combo2->SetupAttachment(hitboxTab);

	h_Combo3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("H_Combo3"));
	h_Combo3->SetupAttachment(hitboxTab);

	h_Combo4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("H_Combo4"));
	h_Combo4->SetupAttachment(hitboxTab);

	h_SP_Hitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("h_SP_Hitbox"));
	h_SP_Hitbox->SetupAttachment(hitboxTab);

	dodgeDetectionHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("H_DodgeDetection"));
	dodgeDetectionHitbox->SetupAttachment(hitboxTab);

	soundPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundPlayer"));
	soundPlayer->SetupAttachment(RootComponent);
	
	alive = true;
}

void AHookNFightCharacter::Attack()
{
	if (true != disableInputButMovement && GetWorld()->TimeSeconds - timeSinceLastAttack >= timeOffsetBetweenTwoAttack && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		attack = true;
		timeSinceAttackStarted = GetWorld()->TimeSeconds;

		if (GetWorld()->TimeSeconds - timeSinceLastAttack >= attackOffsetToStopCombo)
		{
			comboCount = 1;
		}

		else if (comboCount >= comboMax)
		{
			comboCount = 1;
		}
	}
}

void AHookNFightCharacter::BeginPlay()
{
	Super::BeginPlay();

	FollowCamera->Init(this);

	if (h_Combo1)
	{
		h_Combo1->SetHiddenInGame(true);
		h_Combo1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (h_Combo2)
	{
		h_Combo2->SetHiddenInGame(true);
		h_Combo2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (h_Combo3)
	{
		h_Combo3->SetHiddenInGame(true);
		h_Combo3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (h_Combo4)
	{
		h_Combo4->SetHiddenInGame(true);
		h_Combo4->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	dodgeDetectionHitbox->SetBoxExtent(FVector(targetRange, targetRange, targetRange), true);

	sp_ScaleSpeed	= sp_ScaleSpeed / 50;
	sp_ScaleMax		= sp_ScaleMax	/ 50;

	spawnPoint =  UGameplayStatics::GetGameMode(GetWorld())->FindPlayerStart(GetController())->GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Pos = %f, %f, %f"), spawnPoint->GetActorLocation().X, spawnPoint->GetActorLocation().Y, spawnPoint->GetActorLocation().Z) );
}

void AHookNFightCharacter::Check_Attack()
{
	switch (comboCount)
	{
	case 1:
		Do_Attack(h_Combo1, 0.25 / attackSpeed, h_Combo_1_Damage);
		break;
	case 2:
		Do_Attack(h_Combo2, 0.2 / attackSpeed, h_Combo_2_Damage);
		break;
	case 3:
		Do_Attack(h_Combo3, 0.3 / attackSpeed, h_Combo_3_Damage);
		break;
	case 4:
		Do_Attack(h_Combo4, 0.25 / attackSpeed, h_Combo_4_Damage);
		break;
	}

}

void AHookNFightCharacter::CheckForTarget()
{
	if (disableInputButMovement == false)
	{
		FVector vect = FollowCamera->GetForwardVector();
		vect.Normalize();

		FVector targetStartPos = GetActorLocation();
		targetStartPos.Z = FollowCamera->GetComponentLocation().Z;

		float dist2 = FVector::Dist(FollowCamera->GetComponentLocation(), GetActorLocation());

		FHitResult hit;
		FCollisionQueryParams mask = FCollisionQueryParams("test", false, this);
		GetWorld()->LineTraceSingleByChannel(hit, targetStartPos, FollowCamera->GetComponentLocation() + (vect * (targetRange + dist2)), ECollisionChannel::ECC_GameTraceChannel1, mask);

		FHitResult hit2;
		GetWorld()->LineTraceSingleByChannel(hit2, targetStartPos, FollowCamera->GetComponentLocation() + (vect * (targetRange + dist2)), ECollisionChannel::ECC_Visibility, mask);

		if (hit.GetActor() == hit2.GetActor() && hit.GetActor())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CHECK")));
			float dist = FVector::Dist(targetStartPos, hit.GetActor()->GetActorLocation());

			AC_Enemy* enemy = Cast<AC_Enemy>(hit.GetActor());

			if (enemy == nullptr || enemy->Grabbable && enemy->alive)
			{
				if (dist >= grappleRangeMin && dist <= targetRange)
				{
					SetTarget(hit.GetActor());
				}
			}
		}

		if (target)
		{
			float dist = FVector::Dist(GetActorLocation(), target->GetActorLocation());
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Dist = %f"), dist));

			if (dist < grappleRangeMin || dist > targetRange)
			{
				SetTarget(nullptr);
			}
		}

		if (true == drawDebugLineTargetSystem)
		{
			DrawDebugLine(GetWorld(), targetStartPos, FollowCamera->GetComponentLocation() + (vect * (targetRange + dist2)), FColor::Blue, permanentLineTargetSystem, 0.1);
		}
	}
}

void AHookNFightCharacter::DEBUGGet_Hit()
{
	Get_Hit(1);
}

void AHookNFightCharacter::DEBUGRespawn()
{
	SetActorLocation(spawnPoint, false, nullptr,ETeleportType::ResetPhysics);
}

void AHookNFightCharacter::DEBUGScaleSphere(float scale)
{
	scale =  scale / 50;

	if (h_SP_Hitbox->GetComponentScale().X + scale >= DEBUGScaleMax / 50)
	{
		h_SP_Hitbox->SetWorldScale3D(FVector(DEBUGScaleMax / 50 , DEBUGScaleMax / 50, DEBUGScaleMax / 50));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius = %f"), h_SP_Hitbox->GetComponentScale().X * 50));
	}

	else if (h_SP_Hitbox->GetComponentScale().X + scale > 0.1)
	{
		h_SP_Hitbox->SetWorldScale3D(FVector(h_SP_Hitbox->GetComponentScale().X + scale, h_SP_Hitbox->GetComponentScale().X + scale, h_SP_Hitbox->GetComponentScale().X + scale));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius = %f"), h_SP_Hitbox->GetComponentScale().X * 50));
	}

	else
	{
		h_SP_Hitbox->SetWorldScale3D(FVector(0.1, 0.1, 0.1));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius = %f"), h_SP_Hitbox->GetComponentScale().X * 50));
	}
}

void AHookNFightCharacter::DisableInput2()
{
	if (alreadyLaunchedGrapple == true || hitState == true || superAttack == true || alive == false || (attack == true && moveLockWhileAttacking == true) )
	{
		disableInput2 = true;
		disableInputButMovement = true;
	}

	else if ( (attack == true && moveLockWhileAttacking == false) )
	{
		disableInput2 = false;
		disableInputButMovement = true;
	}

	else
	{
		disableInput2 = false;
		disableInputButMovement = false;
	}
}

void AHookNFightCharacter::Do_Attack(UStaticMeshComponent* hitbox, float timeBeforeActivation, float damage)
{
	if (GetWorld()->TimeSeconds - timeSinceAttackStarted >= timeBeforeActivation && alreadyHit == false)
	{
		hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		hitbox->UpdateOverlaps(false);

		TArray<AActor*> list;
		hitbox->GetOverlappingActors(list, AC_Enemy::StaticClass());

		if (list.Num() > 0)
		{
			if (hitSuccessfullSound /*&& GetWorld()->GetRealTimeSeconds() - timeAfterLastHitSuccessfullSound >= hitSuccessfullSound->GetDuration()*/)
			{
				soundPlayer->Stop();
				soundPlayer->SetSound(hitSuccessfullSound);
				soundPlayer->Play();
				timeAfterLastHitSuccessfullSound = GetWorld()->GetRealTimeSeconds();
			}

			for (int index = 0; index < list.Num(); index++)
			{
				Cast<AC_Enemy>(list[index])->Get_Hit(damage);
				currentSPAmount += _SPgainedAfterAttackSucces;
			}
		}

		else
		{
			if (hitMissSound /*&& GetWorld()->GetRealTimeSeconds() - timeAfterLastHitMissSound >= hitMissSound->GetDuration()*/)
			{
				soundPlayer->Stop();
				soundPlayer->SetSound(hitMissSound);
				soundPlayer->Play();
				timeAfterLastHitMissSound = GetWorld()->GetRealTimeSeconds();
			}
		}

		alreadyHit = true;
		comboCount++;
	}

	else if (alreadyHit == true)
	{
		hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AHookNFightCharacter::FireGrapple()
{
	if (disableInputButMovement == false)
	{
		if (target == nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Target = nullptr"));
			TArray<AActor*> list;
			dodgeDetectionHitbox->GetOverlappingActors(list, AC_Enemy::StaticClass());

			if (list.Num() != 0)
			{
				FVector2D closerEnemy = FVector2D(-1, targetRange);

				for (int index = 0; index < list.Num(); index++)
				{
					AC_Enemy* enemy = Cast<AC_Enemy>(list[index]);
					float dist = FVector::Dist(GetActorLocation(), enemy->GetActorLocation());

					if (enemy && enemy->Grabbable && enemy->alive && dist < targetRange && dist >= grappleRangeMin && dist < closerEnemy.Y)
					{
						closerEnemy.X = index;
						closerEnemy.Y = dist;
						//GEngine->AddOnScreenDebugMessage(GEngine->ScreenMessages.Num(), 5.f, FColor::Red, FString::Printf(TEXT("Dist = %f"), dist));
					}
				}

				if (closerEnemy.X != -1)
				{
					SetTarget(list[closerEnemy.X]);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Dist = %f"), FVector::Dist(GetActorLocation(), target->GetActorLocation())));
				}
			}
		}

		if (target != nullptr)
		{
			/*FHitResult hit;
			FCollisionQueryParams mask = FCollisionQueryParams("test", false, this);
			GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, mask);*/
			AA_Grapple::Launch(target, grappleSpeed, grappleTowSpeed, GetWorld(), this);
			alreadyLaunchedGrapple = true;

		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Grapple already launched"));
	}
}

void AHookNFightCharacter::Get_Hit(float damage)
{
	if (false == hitState && immune == false)
	{
		immune = true;
		life -= damage;
		ImmuneHighlight(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("life = %f"), life));

		FollowCamera->ShakeCamera(DamageShake);

		if (life <= 0)
		{
			alive = false;
		}

		else
		{
			hitState = true;
		}
	}
}

void AHookNFightCharacter::GrappleAttack()
{
	attack = true;
	timeSinceAttackStarted = GetWorld()->TimeSeconds;
	comboCount = 1;
}

void AHookNFightCharacter::ImmuneHighlight_Implementation(bool state)
{}


void AHookNFightCharacter::LaunchSuperAttack()
{
	if (true != disableInputButMovement && gaugeSPMaxAmount <= currentSPAmount)
	{
		superAttack = true;
		currentSPAmount = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Super Attack!"));
		if (specialAttackSound && GetWorld()->GetRealTimeSeconds() - timeAfterLastSpecialAttackSound >= specialAttackSound->GetDuration())
		{
			soundPlayer->SetSound(specialAttackSound);
			soundPlayer->Play();
			timeAfterLastSpecialAttackSound = GetWorld()->GetRealTimeSeconds();
		}
	}

	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Not Enough Amout of Super Power !"));
	}
}

void AHookNFightCharacter::LookUpAtRate(float Rate)
{
	if (YAxisInverted)
	{
		Rate *= -1;
	}
	// calculate delta for this frame from the rate information
	AddControllerPitchInput( FollowCamera->ControllerPitchLimiter(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()) );
}

void AHookNFightCharacter::MouseLookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
    AddControllerPitchInput( FollowCamera->ControllerPitchLimiter(Rate) );
}

void AHookNFightCharacter::MouseTurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate);
}

void AHookNFightCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && disableInput2 == false)
	{
		AddMovementInput(FollowCamera->Get_CurrentMoveLandmark().GetUnitAxis(EAxis::X), Value);
	}

	FollowCamera->PlayerMoved(!FMath::IsNearlyZero(Value), MovementDirection::MD_Forward);
}

void AHookNFightCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && disableInput2 == false)
	{
		AddMovementInput(FollowCamera->Get_CurrentMoveLandmark().GetUnitAxis(EAxis::Y), Value);
	}

	FollowCamera->PlayerMoved(!FMath::IsNearlyZero(Value), MovementDirection::MD_Right);
}

void AHookNFightCharacter::Regen()
{
	life = maxLife;
}

void AHookNFightCharacter::Settings()
{
	if (true != disableInputButMovement)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Settings !"));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHookNFightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHookNFightCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHookNFightCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AHookNFightCharacter::MouseTurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHookNFightCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AHookNFightCharacter::MouseLookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHookNFightCharacter::LookUpAtRate);

	// handle touch devices
	/*PlayerInputComponent->BindTouch(IE_Pressed, this, &AHookNFightCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHookNFightCharacter::TouchStopped);*/

	PlayerInputComponent->BindAction("Fire_Grapple", IE_Pressed, this, &AHookNFightCharacter::FireGrapple);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHookNFightCharacter::Attack);
	PlayerInputComponent->BindAction("Super_Attack", IE_Pressed, this, &AHookNFightCharacter::LaunchSuperAttack);
	PlayerInputComponent->BindAction("Settings", IE_Pressed, this, &AHookNFightCharacter::Settings);
	PlayerInputComponent->BindAction("Camera_Reset", IE_Pressed, FollowCamera, &UCC_PlayerCamera::CameraResetStart);
	PlayerInputComponent->BindAction("[DEBUG]Get_Hit", IE_Pressed, this, &AHookNFightCharacter::DEBUGGet_Hit);

	DECLARE_DELEGATE_OneParam(FCustomOneParams, const float);
	PlayerInputComponent->BindAction<FCustomOneParams>("[DEBUG]scaleUpSphere", IE_Repeat, this, &AHookNFightCharacter::DEBUGScaleSphere, DEBUGScaleSpeed);
	PlayerInputComponent->BindAction<FCustomOneParams>("[DEBUG]scaleDownSphere", IE_Repeat, this, &AHookNFightCharacter::DEBUGScaleSphere,-DEBUGScaleSpeed);

	PlayerInputComponent->BindAction("[DEBUG]Respawn", IE_Pressed, this, &AHookNFightCharacter::DEBUGRespawn);
	//PlayerInputComponent->BindAction("[DEBUG]Hitbox", IE_Pressed, this, &AHookNFightCharacter::Spawn_Hitbox);
}

void AHookNFightCharacter::SuperAttack(float deltatime)
{
	if (h_SP_Hitbox->GetComponentScale().X + (sp_ScaleSpeed * deltatime * 100) >= sp_ScaleMax)
	{
		//h_SP_Hitbox->SetWorldScale3D(FVector(sp_ScaleMax / 50 , sp_ScaleMax / 50, sp_ScaleMax / 50));
		h_SP_Hitbox->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		superAttack = false;
		immune = false;
		ImmuneHighlight(false);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius = %f"), h_SP_Hitbox->GetComponentScale().X * 50));
	}

	else
	{
		immune = true;
		ImmuneHighlight(true);
		h_SP_Hitbox->SetWorldScale3D(FVector(h_SP_Hitbox->GetComponentScale().X + (sp_ScaleSpeed * deltatime * 100), h_SP_Hitbox->GetComponentScale().X + (sp_ScaleSpeed * deltatime * 100), h_SP_Hitbox->GetComponentScale().X + (sp_ScaleSpeed * deltatime * 100)));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius = %f"), h_SP_Hitbox->GetComponentScale().X * 50));

		FollowCamera->ShakeCamera(SpecialShake * deltatime);
	}
}

void AHookNFightCharacter::TargetHighlight_Implementation(AActor* target2, bool state)
{}

void AHookNFightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBoom->TargetArmLength = cameraRange;

	if (DEBUGautoHit)
	{
		Get_Hit(1);
	}

	DisableInput2();

	FollowCamera->CameraUpdate(DeltaTime);

	CheckForTarget();

	if (superAttack == true)
	{
		SuperAttack(DeltaTime);
	}

	if (attack == true)
	{
		Check_Attack();
	}

	else
	{
		alreadyHit = false;
	}

	if (hitState == false && GetWorld()->TimeSeconds - timeAfterHitAnimation >= immuneTimeAfterGettingHit && disableInputButMovement == false)
	{
		immune = false;
		ImmuneHighlight(false);
	}
}

void AHookNFightCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (XAxisInverted)
	{
		Rate *= -1;
	}
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}