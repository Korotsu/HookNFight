// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusDebug.h"

#include "C_Enemy.h"
#include "SMC_GlowyRotatingInclinedCube.h"
#include "AIRemoteControlPlugin.h"

#include "Components/TextRenderComponent.h"


UEnemyStatusDebug::UEnemyStatusDebug()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ActionSphere = CreateDefaultSubobject<USMC_GlowySphere>(TEXT("ActionSphere"));
	ActionSphere->SetupAttachment(this);
	ActionSphere->SetRelativeScale3D({0.50f, 0.50f, 0.50f});
	ActionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DirectorSphere = CreateDefaultSubobject<USMC_GlowySphere>(TEXT("DirectorSphere"));
	DirectorSphere->SetupAttachment(ActionSphere);
	DirectorSphere->SetRelativeTransform( FTransform(VectorRegister{0, 0, 0}, {0, -50.f, 50.f}, {0.5f, 0.5f, 0.5f}) );
	DirectorSphere->NewTargetLocation({ 0, -50.f, 50.f });
	DirectorSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UEnemyStatusDebug::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AC_Enemy* EnemyOwner = Cast<AC_Enemy>(GetOwner());

	if		(!EnemyOwner->RemoteControlPlugin->PluggedIn)	DirectorSphere->NewColorTarget({ 0.1f, 0.1f, 0.1f, 1.f });
	else if (EnemyOwner->Attacking)							DirectorSphere->NewColorTarget({1.f, 0.f, 0.f, 1.f});
	else													DirectorSphere->NewColorTarget({0, 0.5f, 1.f, 1});

	if		(EnemyOwner->RemoteControlPlugin->PluggedIn)	DirectorSphere->NewGlowTarget(500.f);
	else													DirectorSphere->NewGlowTarget(0.f);


	if		(!EnemyOwner->alive)			ActionSphere->NewColorTarget({0.1f, 0.1f, 0.1f, 1.f});
	else if (EnemyOwner->Is_Hit())			ActionSphere->NewColor({1.f, 1.f, 0.f, 1.f});
	else if (EnemyOwner->Is_Attacking())	ActionSphere->NewColorTarget({1.f, 0.f, 0.f, 1.f});
	else									ActionSphere->NewColorTarget({0.f, 0.5f, 1.f, 1.f});

	if		(EnemyOwner->Active && EnemyOwner->alive)	ActionSphere->NewGlowTarget(500.f);
	else												ActionSphere->NewGlowTarget(0.f);

	ActionSphere->Update(DeltaTime); 
	DirectorSphere->Update(DeltaTime);
}
