// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomData.h"

URoomPreset::URoomPreset()
{
	//PresetID = FMath::Rand();
}

void URoomPreset::DoThings()
{
	UE_LOG(LogTemp, Warning, TEXT("I am Doing Things"))
}
