// Fill out your copyright notice in the Description page of Project Settings.


#include "TB_SpawnPoint.h"
#include "HookNFightCharacter.h"

ATB_SpawnPoint::ATB_SpawnPoint()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATB_SpawnPoint::OnOverlapBegin);

    spawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
    spawnPoint->SetupAttachment(RootComponent);
}

void ATB_SpawnPoint::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this))
    {
        AHookNFightCharacter* player = Cast<AHookNFightCharacter>(OtherActor);
        if (player)
        {
            player->spawnPoint = spawnPoint->GetComponentLocation();
        }

    }
}
