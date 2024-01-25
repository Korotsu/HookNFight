// Fill out your copyright notice in the Description page of Project Settings.


#include "AIRemoteControlPlugin.h"

#include "CE_MeleeEnemy.h"
#include "CE_ArcherEnemy.h"
#include "AIControlDirectorUnit.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UAIRemoteControlPlugin::UAIRemoteControlPlugin()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	AC_Enemy* Owner = Cast<AC_Enemy>(GetOwner());

	if		( Cast<ACE_MeleeEnemy> (Owner) ) OwnerType = ACE_MeleeEnemy::StaticClass();
	else if ( Cast<ACE_ArcherEnemy>(Owner) ) OwnerType = ACE_ArcherEnemy::StaticClass();
}


void UAIRemoteControlPlugin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	switch (EndPlayReason)
	{
	case (EEndPlayReason::Destroyed):
		
		if (Cast<AC_Enemy>(GetOwner())->Get_AttackOrder() == true)
		{ LinkedDirector->RCP_AttackOverNotification(OwnerType); }
		
		Unplug();
		
		break;

	default:
		break;
	}


	Super::EndPlay(EndPlayReason);
}

void UAIRemoteControlPlugin::Unplug()
{ 
	if (PluggedIn)
	{
		LinkedDirector->RCP_Plugout(this);
		
		LinkedDirector = nullptr;
		PluggedIn = false;
	}
}


FPlugActionInfo UAIRemoteControlPlugin::AIP_StartAttack()
{ 
	AC_Enemy* Owner = Cast<AC_Enemy>(GetOwner());
	
	if (!Owner->Is_Attacking() && !Owner->Is_Hit())
	{
		Owner->BeginAttack(); 
		return FPlugActionInfo(&OwnerType, true);
	}
	else
	{
		return FPlugActionInfo(&OwnerType, false);
	}
}


bool UAIRemoteControlPlugin::AIP_IsAttacking()
{ return Cast<AC_Enemy>(GetOwner())->Is_Attacking(); }

bool UAIRemoteControlPlugin::AIP_IsHit()
{ return Cast<AC_Enemy>(GetOwner())->Is_Hit(); }

bool UAIRemoteControlPlugin::AIP_CanAttack()
{
	AC_Enemy* Owner = Cast<AC_Enemy>(GetOwner());

	return ( !Owner->Is_Attacking() && 
			 !Owner->Is_Hit()		&& 
			  Owner->Is_Active()	);
}


void UAIRemoteControlPlugin::AIP_NotifyEndAttack()
{ LinkedDirector->RCP_AttackOverNotification(OwnerType); }


bool UAIRemoteControlPlugin::Replug(const int& NewGroupNumber)
{
	if (PluggedIn)
	{
		// Case where the RCP is already connected to the correct AIcDu
		if (LinkedDirector->GroupNumber == GroupNumber) return true;

		// Otherwise we unplug from the previous AIcDu
		else
		{
			LinkedDirector->RCP_Plugout(this);

			LinkedDirector	= nullptr, 
			PluggedIn		= false;
		}
	}


	GroupNumber	= NewGroupNumber;

	TArray<AActor*> Directors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIControlDirectorUnit::StaticClass(), Directors);

	for (AActor* Dir : Directors)
	{
		AAIControlDirectorUnit* ADirector = Cast<AAIControlDirectorUnit>(Dir);

		if (ADirector->GroupNumber == GroupNumber)
		{
			LinkedDirector = ADirector;
			LinkedDirector->RCP_Register(this);
			PluggedIn = true;
			return true;
		}
	}

	return false;
}

bool UAIRemoteControlPlugin::Replug()
{
	if (PluggedIn) return true;


	TArray<AActor*> Directors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIControlDirectorUnit::StaticClass(), Directors);

	for (AActor* Dir : Directors)
	{
		AAIControlDirectorUnit* ADirector = Cast<AAIControlDirectorUnit>(Dir);

		if (ADirector->GroupNumber == GroupNumber)
		{
			LinkedDirector = ADirector;
			LinkedDirector->RCP_Register(this);
			PluggedIn = true;
			return true;
		}
	}

	return false;
}

