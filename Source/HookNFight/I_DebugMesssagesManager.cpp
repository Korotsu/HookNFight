// Fill out your copyright notice in the Description page of Project Settings.


#include "I_DebugMesssagesManager.h"

#include "Engine/Engine.h"

AI_DebugMesssagesManager::AI_DebugMesssagesManager()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_LastDemotable);
}

void AI_DebugMesssagesManager::Tick(float DeltaTime)
{
	for (int i = DebugMessages.Num() -1; i >= 0; i--)
	{ GEngine->AddOnScreenDebugMessage( i,  0.1f, DebugMessages[i].Color, DebugMessages[i].String ); }

	DebugMessages.Empty();
}
