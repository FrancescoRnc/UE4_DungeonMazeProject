// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameInstance.h"

UDungeonGameInstance::UDungeonGameInstance()
{
	ConstructorHelpers::FObjectFinder<UInteractableData> tmpI(TEXT("InteractableData'/Game/DataAssets/DA_DungeonDoor.DA_DungeonDoor'"));
	InteractabeAssets.Add("Door", tmpI.Object);

	ConstructorHelpers::FObjectFinder<ULevelRoomContentInfo> tmpR(TEXT("LevelRoomContentInfo'/Game/DataAssets/DA_GeneratedRoom.DA_GeneratedRoom'"));
	RoomAssets.Add("Generated", tmpR.Object);
}

void UDungeonGameInstance::SaveDungeonRoomsData(const FDungeonInfo& dungeon, const TArray<FRoomInfo>& rooms)
{
	sDungeonInfo = dungeon;
	
	for (int i = 0; i < rooms.Num(); i++)
	{
		sRoomsInfo.Add(rooms[i].Name, rooms[i]);
	}
}
