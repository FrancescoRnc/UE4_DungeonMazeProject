// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaze.h"

#include "TextFileManager.h"
#include "Engine/LevelStreamingDynamic.h"


// Room Generator
URoomGenerator::URoomGenerator()
{
 	

}


void URoomGenerator::SetEnvironment(UStaticMeshComponent* inComponent)
{
	inComponent->SetStaticMesh(RoomAsset->EnvironmentMesh);
}

void URoomGenerator::BuildDoors(TArray<ADungeonDoor*> inDoors)
{
	
}

void URoomGenerator::BuildOtherActors()
{
	
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// Room
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EnvironmentMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Environment");
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	RoomGenerator = NewObject<URoomGenerator>();
	RoomGenerator->RoomAsset = RoomAsset;
	RoomGenerator->SetEnvironment(EnvironmentMeshComponent);
	
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Dungeon Maze
ADungeonMaze::ADungeonMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

void ADungeonMaze::BeginPlay()
{
	Super::BeginPlay();

	Builder = NewObject<UCampainDungeonBuilder>();
	CreateDungeonInfo(Builder);
	info = Builder->GetDungeon();
	GenerateDungeonFromInfo(info);
}

void ADungeonMaze::SwitchRooms(FName newRoom)
{
	
}

void ADungeonMaze::CreateDungeonInfo(UCampainDungeonBuilder* _builder)
{
	Builder->BuildDungeon_Implementation(GridWidth, GridHeight);
	const auto gridValues = _builder->GetGridValues(GridWidth, GridHeight);
	const auto gridCoords = _builder->GetRoomsCoordsByValues(gridValues);
	
	for (int i = 0; i < gridCoords.Num(); i++)
	{
		_builder->BuildRoom_Implementation(gridCoords[i]);
	}
}

void ADungeonMaze::GenerateDungeonFromInfo(FDungeonInfo dungeoninfo)
{
	// Instantiate all levels and move them by their coordinates
	FNewLevelInstanceParams params;//
	for (int i = 0; i < dungeoninfo.RoomsCount; i++)
	{
		bool _levelInstanceSuccess;
		const FRoomInfo _roominfo = dungeoninfo.Rooms[i];

		params.LevelName = i == 0 ?
			FirstRoomLevelName : i == (dungeoninfo.RoomsCount - 1) ?
				LastRoomLevelName : GenRoomLevelName;
		params.Location = FVector(_roominfo.GridCoords.X * 4000.f, _roominfo.GridCoords.Y * 4000.f, 0);
		params.Rotation = FRotator::MakeFromEuler(params.Location.GetSafeNormal());
		params.OptionalLevelNameOverride = _roominfo.Name.ToString();
		
		auto newLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), params.LevelName,
																params.Location, params.Rotation,
																_levelInstanceSuccess,
																params.OptionalLevelNameOverride);
		RoomInstances.Add(newLevel);
	}
}

void ADungeonMaze::SetLevelVisibility(const int index, const bool value = false)
{
	RoomInstances[index]->SetShouldBeVisible(value);
	
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Campain Dungeon Builder
UCampainDungeonBuilder::UCampainDungeonBuilder()
{
  	
}

TArray<FIntVector> UCampainDungeonBuilder::GetRoomsCoordsByValues(TArray<int> inValues)
{
	TArray<FIntVector> coordinates;
	FIntVector tmp(0,0,0);
	
	const int length = inValues.Num();
	
	for (int i = 0; i < length; i++)
	{
		switch(inValues[i])
		{
			case 1: tmp = {	i / outDungeon.GridWidth, i % outDungeon.GridWidth, 0};
			coordinates.Add(tmp);
			break;
			
			default: break;
		}
	}

	return coordinates;
}

void UCampainDungeonBuilder::BuildDungeon_Implementation(int sizeX, int sizeY)
{
	outDungeon.GridWidth = sizeX;
	outDungeon.GridHeight = sizeY;
}

void UCampainDungeonBuilder::BuildRoom_Implementation(FIntVector coords)
{
	FString _roomname("Room_");
	_roomname.Append(FString::FromInt(outDungeon.Rooms.Num()));
	FName roomName(_roomname);
	
	FRoomInfo roomInfo;

	roomInfo.Name = roomName;
	roomInfo.GridCoords = coords;
	
	outDungeon.Rooms.Add(roomInfo);
	outDungeon.RoomsCount = outDungeon.Rooms.Num();
}

void UCampainDungeonBuilder::BuildDoor_Implementation(FName roomFrom, FName roomTo)
{
	
}

FDungeonInfo& UCampainDungeonBuilder::GetDungeon()
{
	return outDungeon;
}

TArray<int> UCampainDungeonBuilder::GetGridValues(int width, int height)
{
	TArray<int> values;

	//auto rawValues = UTextFileManager::LoadFileToStringArray("");

	//auto length = rawValues.Num();
	//for (int i = 0; i < length; i++)
	//{
	//	TArray<FString> tmp;
	//	rawValues[i].ParseIntoArrayWS(tmp);

	//	values.Append(tmp);
	//}

	values =
	{
	    1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0
	  /*1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0*/
	};
	
	return values;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
