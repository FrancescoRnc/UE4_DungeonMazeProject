// Copyright Epic Games, Inc. All Rights Reserved.


#include "DungeonMazeProjectGameModeBase.h"


ADungeonMazeProjectGameModeBase::ADungeonMazeProjectGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = false;

	
}

void ADungeonMazeProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ADungeonMazeProjectGameModeBase::LoadNewRoom(const FName newRoomName)
{
	if (newRoomName != "")
	{
		FLatentActionInfo info;
		UGameplayStatics::LoadStreamLevel(this, newRoomName, true, true, info);
	}

	
}
