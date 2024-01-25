// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

void ACharacterBase::SetEffectVolume(float value)
{
	effectVolume = value;
	soundPlayer->SetVolumeMultiplier(value);
}
