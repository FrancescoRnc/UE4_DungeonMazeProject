// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMaze.h"

#include "DungeonGameInstance.h"
#include "DungeonMazeProject/DungeonMazeProjectGameModeBase.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Room
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	WallsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Walls");
	
	PlatformMeshComponent->SetupAttachment(RootComponent);
	WallsMeshComponent->SetupAttachment(RootComponent);

	
	Doors = {
		{ERoomCardinals::NORTH, nullptr},		
		{ERoomCardinals::EAST, nullptr},
		{ERoomCardinals::WEST, nullptr},
		{ERoomCardinals::SOUTH, nullptr},
	};
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	//WallsMeshComponent->SetRenderCustomDepth(false);
}



void ARoom::GenerateRoom(const FRoomInfo& _info)
{
	Info = _info;

	PlatformMeshComponent->SetStaticMesh(Info.RoomAsset->PlatformMesh);
	WallsMeshComponent->SetStaticMesh(Info.RoomAsset->WallMesh);
	
	Transparency = WallsMeshComponent->CreateDynamicMaterialInstance(1, Info.RoomAsset->WallsMaterial);

	//Transparency->SetScalarParameterValue(TEXT("Alpha"), 1.f);
}

TArray<ADungeonDoor*> ARoom::GenerateDoors(const TArray<FDoorInfo>& _info)
{
	TArray<ADungeonDoor*> _doors;
	
	const auto doorAsset = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InteractabeAssets["Door"];
	const FVector _actorlocation = GetActorLocation();
	const FVector _actorforward = GetActorForwardVector();
	const FVector _actorright = GetActorRightVector();
	const int _infolenght = _info.Num();

	FVector loc{};
	FRotator rot{};
	
	for (int i = 0; i < _infolenght; i++)
	{		
		loc = {};
		rot = {};
		
		switch (_info[i].Direction)
		{
			case ERoomCardinals::NORTH:
				if (PlatformMeshComponent->DoesSocketExist("North"))
				{
					loc = PlatformMeshComponent->GetSocketLocation("North");
                    rot = PlatformMeshComponent->GetSocketRotation("North");
				}
				else
				{
					loc = _actorlocation + (_actorforward * 1900.f);
                    rot = FRotator::MakeFromEuler({0, 0, 0});
				}				
				break;
			case ERoomCardinals::SOUTH:
				if (PlatformMeshComponent->DoesSocketExist("South"))
				{
					loc = PlatformMeshComponent->GetSocketLocation("South");
					rot = PlatformMeshComponent->GetSocketRotation("South");
				}
				else
				{
					loc = _actorlocation + (_actorforward * -1900.f);
					rot = FRotator::MakeFromEuler({0, 0, 180});
				}
				break;
			case ERoomCardinals::EAST:
				if (PlatformMeshComponent->DoesSocketExist("East"))
				{
					loc = PlatformMeshComponent->GetSocketLocation("East");
					rot = PlatformMeshComponent->GetSocketRotation("East");
				}
				else
				{
					loc = _actorlocation + (_actorright * 1900.f);
					rot = FRotator::MakeFromEuler({0, 0, 90});
				}
				break;
			case ERoomCardinals::WEST:
				if (PlatformMeshComponent->DoesSocketExist("West"))
				{
					loc = PlatformMeshComponent->GetSocketLocation("West");
					rot = PlatformMeshComponent->GetSocketRotation("West");
				}
				else
				{
					loc = _actorlocation + (_actorright * -1900.f);
					rot = FRotator::MakeFromEuler({0, 0, -90});
				}
				break;
			default: break;
		}		
		
		FActorSpawnParameters spawn;
		ADungeonDoor* newDoor = GetWorld()->SpawnActor<ADungeonDoor>(loc, rot, spawn);
		newDoor->Initialize(_info[i]);		
		
		Doors[newDoor->Info.Direction] = newDoor;
		_doors.Add(newDoor);
	}
	
	return _doors;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Rooms Linker
ARoomLinker::ARoomLinker()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Component");
	
	DoorOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door One"));
	DoorOne->SetupAttachment(RootComponent);
	
	DoorTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Two"));
	DoorTwo->SetupAttachment(RootComponent);
	
	TriggerOne = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger One"));
	TriggerOne->SetupAttachment(DoorOne);
	TriggerOne->SetBoxExtent({50, 60, 100});
    TriggerOne->AddRelativeLocation({25, 0, 0});
	TriggerOne->SetCollisionProfileName(TEXT("Interaction"));
	TriggerOne->SetGenerateOverlapEvents(true);
	TriggerOne->OnComponentBeginOverlap.AddDynamic(this, &ARoomLinker::OnFirstTriggerBeginOverlap);
	
	TriggerTwo = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Two"));	
	TriggerTwo->SetupAttachment(DoorTwo);
	TriggerTwo->SetBoxExtent({50, 60, 100});
	TriggerTwo->AddRelativeLocation({25, 0, 0});
	TriggerTwo->SetCollisionProfileName(TEXT("Interaction"));
	TriggerTwo->SetGenerateOverlapEvents(true);
	TriggerTwo->OnComponentBeginOverlap.AddDynamic(this, &ARoomLinker::OnSecondTriggerBeginOverlap);
}

void ARoomLinker::BeginPlay()
{
	Super::BeginPlay();
}


void ARoomLinker::LocateDoors(const FIntVector4 _roomsinfopack)
{
	RoomsInfoPack = _roomsinfopack;
	
	const TArray<FVector> _cardinals = {
		{1, 0, 0},
		{0, 1, 0},
		{0, -1, 0},
		{-1, 0, 0}
	};

	FVector loc1 = GetActorLocation() + (_cardinals[RoomsInfoPack.Z] * 550);
	FVector loc2 = GetActorLocation() + (_cardinals[RoomsInfoPack.W] * 550);
	
	DoorOne->SetWorldLocationAndRotation(loc1, _cardinals[RoomsInfoPack.Z].Rotation());
	DoorTwo->SetWorldLocationAndRotation(loc2, _cardinals[RoomsInfoPack.W].Rotation());
}

void ARoomLinker::OnFirstTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GoToSecondRoom();
}

void ARoomLinker::OnSecondTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GoToFirstRoom();
}

void ARoomLinker::GoToFirstRoom()
{
	auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector newpos = DoorOne->GetComponentLocation() + (DoorOne->GetForwardVector() * 200) + FVector(0, 0, 100);
	
	player->SetActorLocation(newpos);
}

void ARoomLinker::GoToSecondRoom()
{
	auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector newpos = DoorTwo->GetComponentLocation() + (DoorTwo->GetForwardVector() * 200) + FVector(0, 0, 100);
	
	player->SetActorLocation(newpos);
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

	BuildAndGenerate();
}

void ADungeonMaze::CreateDungeonInfo()
{
	const auto gameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//const auto _genroomasset = gameInstance->RoomAssets["Generated"];
	TArray<ULevelRoomContentInfo*> _roomassets =
	{
		gameInstance->RoomAssets["Start"],
		gameInstance->RoomAssets["Generated"],
		gameInstance->RoomAssets["End"]
	};
	const auto _doorasset = gameInstance->InteractabeAssets["Door"];
	
	Builder->BuildDungeon_Implementation(GridWidth, GridHeight);
	const auto gridValues = Builder->GetGridValues(GridWidth, GridHeight);
	const auto gridCoords = Builder->GetRoomsCoordsByValues(gridValues);
	Builder->BuildRooms_Implementation(_roomassets, gridValues);
	Builder->BuildDoors_Implementation(_doorasset, gridValues);
	
}

void ADungeonMaze::GenerateDungeonFromInfo(const FDungeonInfo& dungeoninfo)
{
	auto _world = GetWorld();
	
	FNewLevelInstanceParams params;
	for (int i = 0; i < dungeoninfo.RoomsCount; i++)
	{
		const FRoomInfo _roominfo = dungeoninfo.Rooms[i];

		const FVector _pos = FVector(_roominfo.GridCoords.X * (4000.f + 200.f),
									 -(_roominfo.GridCoords.Y * (4000.f + 200.f)), 0);
		const FRotator _rot = FVector::ForwardVector.Rotation();
		FActorSpawnParameters parameters;
		parameters.Name = _roominfo.Name;

		auto newRoom = _world->SpawnActor<ARoom>(_pos, _rot, parameters);

		newRoom->GenerateRoom(dungeoninfo.Rooms[i]);
		auto _doors =
			newRoom->GenerateDoors(dungeoninfo.Rooms[i].Doors);
				
		refRooms.Add(_roominfo.Name, newRoom);
		refAllDoors.Append(_doors);
 	}
	
	for (auto door : refAllDoors)
	{
		ARoom* refnextroom = refRooms[door->Info.RoomDestinationName];

		const uint8 cardinalscount = (uint8)(ERoomCardinals::MAX);
		const uint8 currentcardinal = (uint8)(door->Info.Direction);
		const uint8 intoppositecardinal = (cardinalscount - 1) - currentcardinal;

		const ERoomCardinals oppositecardinal = (ERoomCardinals)intoppositecardinal;
		door->LinkedDoor = refnextroom->Doors[oppositecardinal];
	}
}

void ADungeonMaze::BuildAndGenerate()
{
	Builder = NewObject<UDungeonInfoBuilder>();
	CreateDungeonInfo();
	info = Builder->GetDungeon();
	
	auto gameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameInstance->SaveDungeonRoomsData(info, info.Rooms);
	gameInstance->refDungeon = this;
	
	GenerateDungeonFromInfo(info);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Campain Dungeon Builder
UDungeonInfoBuilder::UDungeonInfoBuilder()
{
  	outDungeon = {};
}

void UDungeonInfoBuilder::BuildDungeon_Implementation(const int sizeX, const int sizeY)
{
	outDungeon.GridWidth = sizeX;
	outDungeon.GridHeight = sizeY;
}

void UDungeonInfoBuilder::BuildRooms_Implementation(const TArray<ULevelRoomContentInfo*>& _assets, const TArray<int>& inGrid)
{
	// .X -> x; .Y -> y; .Z -> index in grid; .W -> value on grid[index] 
	TArray<FIntVector4> _coordsPack;
	FIntVector4 tmp(0, 0, 0, 0);
	
	const int _gridlength = inGrid.Num();	
	for (int i = 0; i < _gridlength; i++)
	{
		if (inGrid[i] > 0)
		{
			tmp = {	i / outDungeon.GridWidth,
						i % outDungeon.GridWidth,
						i,
						(inGrid[i] - 1)
			};			
			_coordsPack.Add(tmp);
		}
	}

	const int _coordslenght = _coordsPack.Num();
	for (int i = 0; i < _coordslenght; i++)
	{
		FString _roomname("Room_");
        _roomname.Append(FString::FromInt(i));
        FName roomName(_roomname);
		const int gridindex = _coordsPack[i].Z;
        
        FRoomInfo roomInfo;		
        roomInfo.Name = roomName;
		roomInfo.RoomAsset = _assets[_coordsPack[i].W];
		roomInfo.GridIndex = gridindex;
        roomInfo.GridCoords = {_coordsPack[i].X, _coordsPack[i].Y, 0};
		roomInfo.RoomSize = _assets[_coordsPack[i].W]->RoomSize;
        
        outDungeon.Rooms.Add(roomInfo);
        
	}
	outDungeon.RoomsCount = outDungeon.Rooms.Num();
}

void UDungeonInfoBuilder::BuildDoors_Implementation(UInteractableData* _asset, const TArray<int>& inGrid)
{
	const int width = outDungeon.GridWidth;
	const int height = outDungeon.GridHeight;
	
	for (int i = 0; i < outDungeon.RoomsCount; i++)
	{
		// Current room info
		FRoomInfo& room = outDungeon.Rooms[i];
		const FIntVector currCoords = room.GridCoords;
		const int currGridIndex = room.GridIndex; //room.GridCoords.X + (room.GridCoords.Y * width);

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

FDungeonInfo& UDungeonInfoBuilder::GetDungeon()
{
	return outDungeon;
}

TArray<int> UDungeonInfoBuilder::GetGridValues(const int width, const int height)
{
	TArray<int> values;

	//Here's a REAL implementation:

	//--------------------

	
	// This is a FAKE implementation, just a temporary one.
	values =
	{
	    1, 0, 2, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
		0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 2, 0, 0, 2, 2, 2, 0, 0, 0,
		0, 2, 2, 2, 2, 0, 2, 0, 0, 0,
		0, 2, 0, 0, 2, 0, 2, 0, 0, 0,
		0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
		0, 0, 0, 2, 2, 2, 2, 0, 0, 0,
		0, 2, 2, 2, 0, 0, 0, 0, 0, 0,
		0, 3, 0, 0, 0, 0, 0, 0, 0, 0
	};
	//--------------------
	
	return values;
}

TArray<FIntVector> UDungeonInfoBuilder::GetRoomsCoordsByValues(const TArray<int>& inValues)
{
	TArray<FIntVector> coordinates;
	FIntVector tmp(0,0,0);
	
	const int length = inValues.Num();
	
	for (int i = 0; i < length; i++)
	{
		switch(inValues[i])
		{
		case 1: tmp = {	i / outDungeon.GridWidth, i % outDungeon.GridWidth, i};
			coordinates.Add(tmp);
			break;
			
		default: break;
		}
	}

	return coordinates;
}

const FRoomInfo UDungeonInfoBuilder::GetRoomByGridIndex(const TArray<FRoomInfo>& rooms, const int gridindex) const
{
	FRoomInfo room{};

	const int _roomscount = rooms.Num();
	for (int i = 0; i < _roomscount; i++)
	{
		if (rooms[i].GridIndex == gridindex)
			room = rooms[i];
	}

	return room;
}

const FRoomInfo UDungeonInfoBuilder::GetRoomByCoords(const TArray<FRoomInfo>& rooms, const int x, const int y) const
{
	FRoomInfo room{};

	const int _roomscount = rooms.Num();
	for (int i = 0; i < _roomscount; i++)
	{
		if (rooms[i].GridCoords.X == x && rooms[i].GridCoords.Y == y)
			room = rooms[i];
	}

	return room;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Dungeon Generator
ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent	= CreateDefaultSubobject<USceneComponent>("Root Component");
	FloorInstances	= CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floors"));
	WallInstances	= CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
	DoorInstances	= CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Doors"));
}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	Generate();

	TArray<FIntVector> _roomcouples;
	
	LinkRooms(_roomcouples);
}

void ADungeonGenerator::Generate()
{
	if (GridWidth <= 0 && GridHeight <= 0) return;

	FloorInstances->	ClearInstances();
	WallInstances->		ClearInstances();
	//DoorInstances->		ClearInstances();
	
	const int length = GridWidth * GridHeight;

	TArray<FTransform> transforms;
	TArray<FTransform> doortransforms;
	transforms.Init(GetActorTransform(), length);	
	doortransforms.Init(GetActorTransform(), length * 4);

	const TArray<FIntVector> cardinals = {
		{1, 0, 0},
		{0, 1, 0},
		{0, -1, 0},
		{-1, 0, 0}
	};
	
	for (int i = 0; i < length; i++)
	{
		const FIntVector currcoords = {i % GridHeight, i / GridHeight, 0};
		
		const FVector currloc = GetActorLocation() + FVector(currcoords.X * 5000.f, -currcoords.Y * 5000.f, 0);
		
		transforms[i].SetLocation(currloc);

		for (int j = 0; j < 4; j++)
		{
			const FVector currdoorloc;
			const FQuat currdoorrot;
			doortransforms[i * j].SetLocation(currdoorloc);
			doortransforms[i * j].SetRotation(currdoorrot);
		}
	}
	
	FloorInstances->	AddInstances(transforms, false);
    WallInstances->		AddInstances(transforms, false);
	//DoorInstances->		AddInstances(doortransforms, false);

}

void ADungeonGenerator::LinkRooms(const TArray<FIntVector>& _inroomcouples)
{
	auto _world = GetWorld();

	const auto length = _inroomcouples.Num();
	for (int i = 0; i < length; i++)
	{
		FVector pos = {};
		FRotator rot = GetActorForwardVector().Rotation();
		
		ARoomLinker* linker = _world->SpawnActor<ARoomLinker>(pos, rot);
		
		linker->LocateDoors({_inroomcouples[i].X, _inroomcouples[i].Y, 0, 0});
	}
}


