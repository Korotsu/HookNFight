// Fill out your copyright notice in the Description page of Project Settings.


#include "TB_KillZone.h"
#include "HookNFightCharacter.h"
#include "C_Enemy.h"

ATB_KillZone::ATB_KillZone()
{
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATB_KillZone::OnOverlapBegin);
}

void ATB_KillZone::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this))
    {
        AHookNFightCharacter* player = Cast<AHookNFightCharacter>(OtherActor);
        if (player)
        {
            player->Get_Hit(damage);
            player->SetActorLocation(player->spawnPoint, false, nullptr, ETeleportType::ResetPhysics);
        }

        AC_Enemy* enemy = Cast<AC_Enemy>(OtherActor);
        if (enemy)
        {
            enemy->Destroy();
        }


    }
}
