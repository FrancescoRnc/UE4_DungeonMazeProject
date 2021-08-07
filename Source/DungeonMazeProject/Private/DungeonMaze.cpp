// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaze.h"

//#include "DungeonCharacter.h"

#include "DungeonGameInstance.h"
#include "TextFileManager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Room Generator
URoomGenerator::URoomGenerator()
{
 	

}


void URoomGenerator::GenerateRoom(ARoom* inRoom, const FRoomInfo& _info)
{
	RoomAsset = _info.RoomAsset;
	SetEnvironment(inRoom->EnvironmentMeshComponent);
	
}

void URoomGenerator::SetEnvironment(UStaticMeshComponent* inComponent)
{
	inComponent->SetStaticMesh(RoomAsset->EnvironmentMesh);
}

void URoomGenerator::GenerateDoors(const FRoomInfo& inInfo, TArray<ADungeonDoor*>& inDoors, UStaticMesh* mesh)
{
	// TO DO
	for (int i = 0; i < inInfo.Doors.Num(); i++)
	{
		FVector loc(0,0,0);
     	FRotator rot(0,0,0);
     	FActorSpawnParameters spawn;
		auto world = GetWorld();
		ADungeonDoor* newDoor = world->SpawnActor<ADungeonDoor>(loc, rot, spawn);

		
		
		newDoor->AddActorLocalOffset({1700.f, 0, 0});
		//UStaticMeshSocket* socket;
		//TArray<ERoomCardinals> keys;
		//inInfo.Doors.GetKeys(keys);
		
		//UE_LOG(LogTemp, Warning, TEXT(""));
		//uint8 correctCardinal = (int)ERoomCardinals::NORTH;
		//mesh->Sockets[correctCardinal]->AttachActor(newDoor, newDoor->MeshComponent);
		inDoors.Add(newDoor);
	}
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
	SetRootComponent(EnvironmentMeshComponent);
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	RoomGenerator = NewObject<URoomGenerator>();

	//Info = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->sRoomsInfo["Room_1"];
	
	//RoomGenerator->GenerateRoom(this, Info);

	/*FRoomInfo fakeInfo = {
		"Room_99",
		{0,0,0},
		{10.f, 10.f},
		{
				{ERoomCardinals::NORTH, "LOL"},
				{ERoomCardinals::SOUTH, "LOL"},
				{ERoomCardinals::EAST, "LOL"},
				{ERoomCardinals::WEST, "LOL"}
		}
	};*/

	//GenerateDoors();
}

void ARoom::GenerateRoom(const FRoomInfo& _info)
{
	Info = _info;
	//RoomAsset = _asset;

	EnvironmentMeshComponent->SetStaticMesh(_info.RoomAsset->EnvironmentMesh);
	
	
}

void ARoom::GenerateDoors()
{
	auto doorAsset = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InteractabeAssets["Door"];
	for (int i = 0; i < Info.Doors.Num(); i++)
	{
		const FVector _actorLocation = GetActorLocation();
		FVector loc;
     	FRotator rot;

		
		
		switch (Info.Doors[i].Direction)
		{
			case ERoomCardinals::NORTH:
				if (EnvironmentMeshComponent->DoesSocketExist("North"))
				{
					loc = EnvironmentMeshComponent->GetSocketLocation("North");
                    rot = EnvironmentMeshComponent->GetSocketRotation("North");
				}
				else
				{
					loc = _actorLocation + FVector{1700.f, 0, 0};
                    rot = FRotator::MakeFromEuler({0, 0, 0});
				}				
				break;
			case ERoomCardinals::SOUTH:
				if (EnvironmentMeshComponent->DoesSocketExist("South"))
				{
					loc = EnvironmentMeshComponent->GetSocketLocation("South");
					rot = EnvironmentMeshComponent->GetSocketRotation("South");
				}
				else
				{
					loc = _actorLocation + FVector{-1700.f, 0, 0};
					rot = FRotator::MakeFromEuler({0, 0, 180});
				}
				break;
			case ERoomCardinals::EAST:
				if (EnvironmentMeshComponent->DoesSocketExist("East"))
				{
					loc = EnvironmentMeshComponent->GetSocketLocation("East");
					rot = EnvironmentMeshComponent->GetSocketRotation("East");
				}
				else
				{
					loc = _actorLocation + FVector{0, 1700.f, 0};
					rot = FRotator::MakeFromEuler({0, 0, 90});
				}
				break;
			case ERoomCardinals::WEST:
				if (EnvironmentMeshComponent->DoesSocketExist("West"))
				{
					loc = EnvironmentMeshComponent->GetSocketLocation("West");
					rot = EnvironmentMeshComponent->GetSocketRotation("West");
				}
				else
				{
					loc = _actorLocation + FVector{0, -1700.f, 0};
					rot = FRotator::MakeFromEuler({0, 0, -90});
				}
				break;
			default: break;
		}		
		
		FActorSpawnParameters spawn;
		ADungeonDoor* newDoor = GetWorld()->SpawnActor<ADungeonDoor>(loc, rot, spawn);
		newDoor->Initialize(Info.Doors[i]);

		
		
		Doors.Add(newDoor);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Dungeon Maze
ADungeonMaze::ADungeonMaze()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//ConstructorHelpers::FObjectFinder<ULevelRoomContentInfo> asset(TEXT("LevelRoomContentInfo'/Game/DataAssets/DA_GeneratedRoom.DA_GeneratedRoom'"));
	//RoomAsset = asset.Object;
	
}

void ADungeonMaze::BeginPlay()
{
	Super::BeginPlay();

	Builder = NewObject<UCampainDungeonBuilder>();
	CreateDungeonInfo();
	info = Builder->GetDungeon();

	
	auto gameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameInstance->SaveDungeonRoomsData(info, info.Rooms);
	gameInstance->refDungeon = this;
	
	GenerateDungeonFromInfo(info);
}

void ADungeonMaze::SwitchRooms(FName newRoom)
{
	
}

void ADungeonMaze::CreateDungeonInfo()
{
	auto gameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto _roomasset = gameInstance->RoomAssets["Generated"];
	auto _doorasset = gameInstance->InteractabeAssets["Door"];
	
	Builder->BuildDungeon_Implementation(GridWidth, GridHeight);
	const auto gridValues = Builder->GetGridValues(GridWidth, GridHeight);
	const auto gridCoords = Builder->GetRoomsCoordsByValues(gridValues);
	Builder->BuildRooms_Implementation(_roomasset, gridCoords);
	Builder->BuildDoors_Implementation(_doorasset, gridValues);
	
}

void ADungeonMaze::GenerateDungeonFromInfo(const FDungeonInfo& dungeoninfo)
{
	// Instantiate all levels and move them by their coordinates
	auto _world = GetWorld();
	
	FNewLevelInstanceParams params;//
	for (int i = 0; i < dungeoninfo.RoomsCount; i++)
	{
		//bool _levelInstanceSuccess;
		const FRoomInfo _roominfo = dungeoninfo.Rooms[i];

		//params.LevelName = i == 0 ?
		//	FirstRoomLevelName : i == (dungeoninfo.RoomsCount - 1) ?
		//		LastRoomLevelName : GenRoomLevelName;
		//params.Location = FVector(_roominfo.GridCoords.X * 4000.f, _roominfo.GridCoords.Y * 4000.f, 0);
		//params.Rotation = FRotator::MakeFromEuler(params.Location.GetSafeNormal());
		//params.OptionalLevelNameOverride = _roominfo.Name.ToString();

		FVector _pos = FVector( _roominfo.GridCoords.X * _roominfo.RoomSize.X,
								-(_roominfo.GridCoords.Y * _roominfo.RoomSize.Y), 0);
		FRotator _rot = FRotator::MakeFromEuler(params.Location.GetSafeNormal());
		FActorSpawnParameters parameters;
		parameters.Name = _roominfo.Name;
		
		//auto newLevel = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), params.LevelName,
		//														params.Location, params.Rotation,
		//														_levelInstanceSuccess,
		//														params.OptionalLevelNameOverride);
		
		//newLevel->SetShouldBeLoaded(false);
		//RoomInstances.Add(newLevel);

		auto newRoom = _world->SpawnActor<ARoom>(_pos, _rot, parameters);

		newRoom->GenerateRoom(dungeoninfo.Rooms[i]);
		newRoom->GenerateDoors();
		
		refRooms.Add(newRoom);
	}
	
}

//void ADungeonMaze::SetLevelVisibility(const int index, const bool value = false)
//{
//	RoomInstances[index]->SetShouldBeVisible(value);
//	
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Campain Dungeon Builder
UCampainDungeonBuilder::UCampainDungeonBuilder()
{
  	
}


void UCampainDungeonBuilder::BuildDoorsForRoom(const TArray<int>& inGrid, const int roomIndex)
{
	
}

void UCampainDungeonBuilder::BuildDungeon_Implementation(const int sizeX, const int sizeY)
{
	outDungeon.GridWidth = sizeX;
	outDungeon.GridHeight = sizeY;
}

/*void UCampainDungeonBuilder::BuildRoom_Implementation(const FIntVector coords)
{
	FString _roomname("Room_");
	_roomname.Append(FString::FromInt(outDungeon.Rooms.Num()));
	FName roomName(_roomname);
	
	FRoomInfo roomInfo;

	roomInfo.Name = roomName;
	roomInfo.GridCoords = coords;
	
	outDungeon.Rooms.Add(roomInfo);
	outDungeon.RoomsCount = outDungeon.Rooms.Num();
}*/
void UCampainDungeonBuilder::BuildRooms_Implementation(ULevelRoomContentInfo* _asset, const TArray<FIntVector>& coords)
{
	for (int i = 0; i < coords.Num(); i++)
	{
		FString _roomname("Room_");
        _roomname.Append(FString::FromInt(i));
        FName roomName(_roomname);
        
        FRoomInfo roomInfo;
		
        roomInfo.Name = roomName;
		roomInfo.RoomAsset = _asset;
        roomInfo.GridCoords = coords[i];
		roomInfo.RoomSize = {4000.f, 4000.f};
        
        outDungeon.Rooms.Add(roomInfo);
        
	}
	outDungeon.RoomsCount = outDungeon.Rooms.Num();
}

//void UCampainDungeonBuilder::BuildDoor_Implementation(const FName& roomFrom, const FName& roomTo)
//{
//	
//}
void UCampainDungeonBuilder::BuildDoors_Implementation(UInteractableData* _asset, const TArray<int>& inGrid)
{
	const int width = outDungeon.GridWidth;
	const int height = outDungeon.GridHeight;
	
	for (int i = 0; i < outDungeon.RoomsCount; i++)
	{
		// Current room info
		FRoomInfo& room = outDungeon.Rooms[i];
		const FIntVector currCoords = room.GridCoords;
		const int currGridIndex = room.GridCoords.X + (room.GridCoords.Y * width);

		// Adjacent room coordinates
		const FIntVector Up{currCoords.X + 1, currCoords.Y, 0};   
		const FIntVector Down{currCoords.X - 1, currCoords.Y, 0}; 
		const FIntVector Right{currCoords.X, currCoords.Y - 1, 0};
		const FIntVector Left{currCoords.X, currCoords.Y + 1, 0}; 
		
		const int UpIndex = Up.Y + (Up.X * width);
		const int DownIndex = Down.Y + (Down.X * width);
		const int RightIndex = Right.Y + (Right.X * width);
		const int LeftIndex = Left.Y + (Left.X * width);
		
		// Check Up
		if (Up.X < height && inGrid[UpIndex])
			room.Doors.Add(	{_asset,
							ERoomCardinals::NORTH,
							room.Name,
							GetRoomByCoords(outDungeon.Rooms, Up.X, Up.Y).Name});
		// Check Down
		if (Down.X >= 0 && inGrid[DownIndex])
			room.Doors.Add( {_asset,
							ERoomCardinals::SOUTH,
							room.Name,
							GetRoomByCoords(outDungeon.Rooms, Down.X, Down.Y).Name});
		// Check Right
		if (Right.Y >= 0 && inGrid[RightIndex])
			room.Doors.Add( {_asset,
							ERoomCardinals::EAST,
							room.Name,
							GetRoomByCoords(outDungeon.Rooms, Right.X, Right.Y).Name});
		// Check Left
		if (Left.Y < width && inGrid[LeftIndex])
			room.Doors.Add( {_asset,
							ERoomCardinals::WEST,
							room.Name,
							GetRoomByCoords(outDungeon.Rooms, Left.X, Left.Y).Name});
	}
}

FDungeonInfo& UCampainDungeonBuilder::GetDungeon()
{
	return outDungeon;
}

TArray<int> UCampainDungeonBuilder::GetGridValues(const int width, const int height)
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
		0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
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

TArray<FIntVector> UCampainDungeonBuilder::GetRoomsCoordsByValues(const TArray<int>& inValues)
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

const FRoomInfo UCampainDungeonBuilder::GetRoomByGridIndex(const TArray<FRoomInfo>& rooms, const int gridindex) const
{
	FRoomInfo room{};

	for (int i = 0; i < rooms.Num(); i++)
	{
		const int x = i / outDungeon.GridWidth;
		const int y = i % outDungeon.GridWidth;

		if (rooms[i].GridCoords == FIntVector(x, y, 0))
			room = rooms[i];
	}

	return room;
}

const FRoomInfo UCampainDungeonBuilder::GetRoomByCoords(const TArray<FRoomInfo>& rooms, const int x, const int y) const
{
	FRoomInfo room{};

	for (int i = 0; i < rooms.Num(); i++)
	{
		if (rooms[i].GridCoords == FIntVector(x, y, 0))
			room = rooms[i];
	}

	return room;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
