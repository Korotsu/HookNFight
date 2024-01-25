// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "I_DebugMesssagesManager.generated.h"



struct FDebugMessage
{
	FColor	Color;
	FString String;

	FDebugMessage(const FColor& _Color, const FString& _String)
		: Color		(_Color),
		  String	(_String)
	{}
};


UCLASS()
class HOOKNFIGHT_API AI_DebugMesssagesManager : public AInfo
{
	GENERATED_BODY()

	TArray<FDebugMessage> DebugMessages;

public:
	AI_DebugMesssagesManager();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void AddMessage(FDebugMessage&& Message)
	{ DebugMessages.Add(Message); }

	FORCEINLINE void AddMessageOnTop(FDebugMessage&& Message)
	{ DebugMessages.EmplaceAt(0, Message); }
};


FORCEINLINE AI_DebugMesssagesManager* GetDMM(UWorld* World)
{ 
	AI_DebugMesssagesManager* DDM = Cast<AI_DebugMesssagesManager>( UGameplayStatics::GetActorOfClass(World, AI_DebugMesssagesManager::StaticClass()) );

	if (DDM == nullptr)
	{ DDM = World->SpawnActor<AI_DebugMesssagesManager>(); }

	return DDM;
}