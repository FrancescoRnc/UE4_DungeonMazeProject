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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	WallsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Walls"));
	
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
	
	const UInteractableData* DoorAsset = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->InteractabeAssets[TEXT("Door")];
	const FVector Actorlocation = GetActorLocation();
	const FVector Actorforward = GetActorForwardVector();
	const FVector Actorright = GetActorRightVector();
	const int Infolenght = _info.Num();

	FVector Location{};
	FRotator Rotation{};
	
	for (int i = 0; i < Infolenght; i++)
	{		
		Location = {};
		Rotation = {};
		FString StrDirection = TEXT("");
		FName DirectionName("");
		
		switch (_info[i].Direction)
		{
			case ERoomCardinals::NORTH:
				StrDirection = TEXT("North");
				DirectionName = *StrDirection;
				//dir = "North";
				if (PlatformMeshComponent->DoesSocketExist(DirectionName))
				{
					Location = PlatformMeshComponent->GetSocketLocation(DirectionName);
                    Rotation = PlatformMeshComponent->GetSocketRotation(DirectionName);
				}
				else
				{
					Location = Actorlocation + (Actorforward * 1900.f);
                    Rotation = FRotator::MakeFromEuler({0, 0, 0});
				}				
				break;
			case ERoomCardinals::SOUTH:
				StrDirection = TEXT("South");
				DirectionName = *StrDirection;
				//dir = "South";
				if (PlatformMeshComponent->DoesSocketExist(DirectionName))
				{
					Location = PlatformMeshComponent->GetSocketLocation(DirectionName);
					Rotation = PlatformMeshComponent->GetSocketRotation(DirectionName);
				}
				else
				{
					Location = Actorlocation + (Actorforward * -1900.f);
					Rotation = FRotator::MakeFromEuler({0, 0, 180});
				}
				break;
			case ERoomCardinals::EAST:
				StrDirection = TEXT("East");
				DirectionName = *StrDirection;
				//dir = "East";
				if (PlatformMeshComponent->DoesSocketExist(DirectionName))
				{
					Location = PlatformMeshComponent->GetSocketLocation(DirectionName);
					Rotation = PlatformMeshComponent->GetSocketRotation(DirectionName);
				}
				else
				{
					Location = Actorlocation + (Actorright * 1900.f);
					Rotation = FRotator::MakeFromEuler({0, 0, 90});
				}
				break;
			case ERoomCardinals::WEST:
				StrDirection = TEXT("West");
				DirectionName = *StrDirection;
				//dir = "West";
				if (PlatformMeshComponent->DoesSocketExist(DirectionName))
				{
					Location = PlatformMeshComponent->GetSocketLocation(DirectionName);
					Rotation = PlatformMeshComponent->GetSocketRotation(DirectionName);
				}
				else
				{
					Location = Actorlocation + (Actorright * -1900.f);
					Rotation = FRotator::MakeFromEuler({0, 0, -90});
				}
				break;
			default: break;
		}		
		
		FActorSpawnParameters SpawnParameters;
		ADungeonDoor* newDoor = GetWorld()->SpawnActor<ADungeonDoor>(Location, Rotation, SpawnParameters);
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
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector newpos = DoorOne->GetComponentLocation() + (DoorOne->GetForwardVector() * 200) + FVector(0, 0, 100);
	
	player->SetActorLocation(newpos);
}

void ARoomLinker::GoToSecondRoom()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
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
	const UDungeonGameInstance* gameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//const auto _genroomasset = gameInstance->RoomAssets["Generated"];
	TArray<ULevelRoomContentInfo*> _roomassets =
	{
		gameInstance->RoomAssets["Start"],
		gameInstance->RoomAssets["Generated"],
		gameInstance->RoomAssets["End"]
	};
	UInteractableData* _doorasset = gameInstance->InteractabeAssets["Door"];
	
	Builder->BuildDungeon_Implementation(GridWidth, GridHeight);
	const TArray<int> gridValues = Builder->GetGridValues(GridWidth, GridHeight);
	const TArray<FIntVector> gridCoords = Builder->GetRoomsCoordsByValues(gridValues);
	Builder->BuildRooms_Implementation(_roomassets, gridValues);
	Builder->BuildDoors_Implementation(_doorasset, gridValues);
	
}

void ADungeonMaze::GenerateDungeonFromInfo(const FDungeonInfo& dungeoninfo)
{
	UWorld* _world = GetWorld();
	
	FNewLevelInstanceParams params;
	for (int i = 0; i < dungeoninfo.RoomsCount; i++)
	{
		const FRoomInfo _roominfo = dungeoninfo.Rooms[i];

		const FVector _pos = FVector(_roominfo.GridCoords.X * (4000.f + 200.f),
									 -(_roominfo.GridCoords.Y * (4000.f + 200.f)), 0);
		const FRotator _rot = FVector::ForwardVector.Rotation();
		FActorSpawnParameters parameters;
		parameters.Name = _roominfo.Name;

		ARoom* NewRoom = _world->SpawnActor<ARoom>(_pos, _rot, parameters);

		NewRoom->GenerateRoom(dungeoninfo.Rooms[i]);
		TArray<ADungeonDoor*> _doors = NewRoom->GenerateDoors(dungeoninfo.Rooms[i].Doors);
				
		RefRooms.Add(_roominfo.Name, NewRoom);
		RefAllDoors.Append(_doors);
 	}
	
	for (ADungeonDoor* Door : RefAllDoors)
	{
		ARoom* RefNextRoom = RefRooms[Door->Info.RoomDestinationName];

		const uint8 CardinalsCount = static_cast<uint8>(ERoomCardinals::MAX);
		const uint8 CurrentCardinal = static_cast<uint8>(Door->Info.Direction);
		const uint8 IntOppositeCardinal = (CardinalsCount - 1) - CurrentCardinal;

		const ERoomCardinals OppositeCardinal = static_cast<ERoomCardinals>(IntOppositeCardinal);
		Door->LinkedDoor = RefNextRoom->Doors[OppositeCardinal];
	}
}

void ADungeonMaze::BuildAndGenerate()
{
	Builder = NewObject<UDungeonInfoBuilder>();
	CreateDungeonInfo();
	Info = Builder->GetDungeon();
	
	UDungeonGameInstance* GameInstance = Cast<UDungeonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SaveDungeonRoomsData(Info, Info.Rooms);
	GameInstance->refDungeon = this;
	
	GenerateDungeonFromInfo(Info);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Campain Dungeon Builder
UDungeonInfoBuilder::UDungeonInfoBuilder()
{
  	OutDungeon = {};
}

void UDungeonInfoBuilder::BuildDungeon_Implementation(const int sizeX, const int sizeY)
{
	OutDungeon.GridWidth = sizeX;
	OutDungeon.GridHeight = sizeY;
}

void UDungeonInfoBuilder::BuildRooms_Implementation(const TArray<ULevelRoomContentInfo*>& _assets, const TArray<int>& inGrid)
{
	// .X -> x; .Y -> y; .Z -> index in grid; .W -> value on grid[index] 
	TArray<FIntVector4> CoordsPack;
	FIntVector4 tmp(0, 0, 0, 0);
	
	const int Gridlength = inGrid.Num();	
	for (int i = 0; i < Gridlength; i++)
	{
		if (inGrid[i] > 0)
		{
			tmp = {	i / OutDungeon.GridWidth,
						i % OutDungeon.GridWidth,
						i,
						(inGrid[i] - 1)
			};			
			CoordsPack.Add(tmp);
		}
	}

	const int Coordslenght = CoordsPack.Num();
	for (int i = 0; i < Coordslenght; i++)
	{
		FString RoomNameStr("Room_");
        RoomNameStr.Append(FString::FromInt(i));
        FName RoomName(*RoomNameStr);
		const int Gridindex = CoordsPack[i].Z;
        
        FRoomInfo RoomInfo;		
        RoomInfo.Name = RoomName;
		RoomInfo.RoomAsset = _assets[CoordsPack[i].W];
		RoomInfo.GridIndex = Gridindex;
        RoomInfo.GridCoords = {CoordsPack[i].X, CoordsPack[i].Y, 0};
		RoomInfo.RoomSize = _assets[CoordsPack[i].W]->RoomSize;
        
        OutDungeon.Rooms.Add(RoomInfo);
        
	}
	OutDungeon.RoomsCount = OutDungeon.Rooms.Num();
}

void UDungeonInfoBuilder::BuildDoors_Implementation(UInteractableData* _asset, const TArray<int>& inGrid)
{
	const int Width = OutDungeon.GridWidth;
	const int Height = OutDungeon.GridHeight;
	
	for (int i = 0; i < OutDungeon.RoomsCount; i++)
	{
		// Current room info
		FRoomInfo& CurrentRoom = OutDungeon.Rooms[i];
		const FIntVector currCoords = CurrentRoom.GridCoords;
		const int currGridIndex = CurrentRoom.GridIndex; //room.GridCoords.X + (room.GridCoords.Y * width);

		// Adjacent room coordinates
		const FIntVector Up{currCoords.X + 1, currCoords.Y, 0};   
		const FIntVector Down{currCoords.X - 1, currCoords.Y, 0}; 
		const FIntVector Right{currCoords.X, currCoords.Y - 1, 0};
		const FIntVector Left{currCoords.X, currCoords.Y + 1, 0}; 
		
		const int UpIndex = Up.Y + (Up.X * Width);
		const int DownIndex = Down.Y + (Down.X * Width);
		const int RightIndex = Right.Y + (Right.X * Width);
		const int LeftIndex = Left.Y + (Left.X * Width);
		
		// Check Up
		if (Up.X < Height && inGrid[UpIndex])
			CurrentRoom.Doors.Add(	{_asset,
							ERoomCardinals::NORTH,
							CurrentRoom.Name,
							GetRoomByCoords(OutDungeon.Rooms, Up.X, Up.Y).Name});
		// Check Down
		if (Down.X >= 0 && inGrid[DownIndex])
			CurrentRoom.Doors.Add( {_asset,
							ERoomCardinals::SOUTH,
							CurrentRoom.Name,
							GetRoomByCoords(OutDungeon.Rooms, Down.X, Down.Y).Name});
		// Check Right
		if (Right.Y >= 0 && inGrid[RightIndex])
			CurrentRoom.Doors.Add( {_asset,
							ERoomCardinals::EAST,
							CurrentRoom.Name,
							GetRoomByCoords(OutDungeon.Rooms, Right.X, Right.Y).Name});
		// Check Left
		if (Left.Y < Width && inGrid[LeftIndex])
			CurrentRoom.Doors.Add( {_asset,
							ERoomCardinals::WEST,
							CurrentRoom.Name,
							GetRoomByCoords(OutDungeon.Rooms, Left.X, Left.Y).Name});
	}
}

FDungeonInfo& UDungeonInfoBuilder::GetDungeon()
{
	return OutDungeon;
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
	TArray<FIntVector> Coordinates;
	FIntVector tmp(0,0,0);
	
	const int length = inValues.Num();
	
	for (int i = 0; i < length; i++)
	{
		switch(inValues[i])
		{
		case 1: tmp = {	i / OutDungeon.GridWidth, i % OutDungeon.GridWidth, i};
			Coordinates.Add(tmp);
			break;
			
		default: break;
		}
	}

	return Coordinates;
}

const FRoomInfo UDungeonInfoBuilder::GetRoomByGridIndex(const TArray<FRoomInfo>& rooms, const int gridindex) const
{
	FRoomInfo RoomInfo{};

	const int RoomsCount = rooms.Num();
	for (int i = 0; i < RoomsCount; i++)
	{
		if (rooms[i].GridIndex == gridindex)
			RoomInfo = rooms[i];
	}

	return RoomInfo;
}

const FRoomInfo UDungeonInfoBuilder::GetRoomByCoords(const TArray<FRoomInfo>& rooms, const int x, const int y) const
{
	FRoomInfo RoomInfo{};

	const int RoomsCount = rooms.Num();
	for (int i = 0; i < RoomsCount; i++)
	{
		if (rooms[i].GridCoords.X == x && rooms[i].GridCoords.Y == y)
			RoomInfo = rooms[i];
	}

	return RoomInfo;
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

	TArray<FIntVector> RoomCouples;
	
	LinkRooms(RoomCouples);
}

void ADungeonGenerator::Generate()
{
	if (GridWidth <= 0 && GridHeight <= 0) return;

	FloorInstances->	ClearInstances();
	WallInstances->		ClearInstances();
	//DoorInstances->		ClearInstances();
	
	const int Length = GridWidth * GridHeight;

	TArray<FTransform> Transforms;
	TArray<FTransform> DoorTransforms;
	Transforms.Init(GetActorTransform(), Length);	
	DoorTransforms.Init(GetActorTransform(), Length * 4);

	const TArray<FIntVector> cardinals = {
		{1, 0, 0},
		{0, 1, 0},
		{0, -1, 0},
		{-1, 0, 0}
	};
	
	for (int i = 0; i < Length; i++)
	{
		const FIntVector CurrCoords = {i % GridHeight, i / GridHeight, 0};
		
		const FVector CurrLoc = GetActorLocation() + FVector(CurrCoords.X * 5000.f, -CurrCoords.Y * 5000.f, 0);
		
		Transforms[i].SetLocation(CurrLoc);

		for (int j = 0; j < 4; j++)
		{
			const FVector CurrDoorLoc;
			const FQuat CurrDoorTot;
			DoorTransforms[i * j].SetLocation(CurrDoorLoc);
			DoorTransforms[i * j].SetRotation(CurrDoorTot);
		}
	}
	
	FloorInstances->	AddInstances(Transforms, false);
    WallInstances->		AddInstances(Transforms, false);
	//DoorInstances->		AddInstances(doortransforms, false);

}

void ADungeonGenerator::LinkRooms(const TArray<FIntVector>& _inroomcouples)
{
	UWorld* World = GetWorld();

	const int32 Length = _inroomcouples.Num();
	for (int i = 0; i < Length; i++)
	{
		FVector Location = {};
		FRotator Rotation = GetActorForwardVector().Rotation();
		
		ARoomLinker* Linker = World->SpawnActor<ARoomLinker>(Location, Rotation);
		
		Linker->LocateDoors({_inroomcouples[i].X, _inroomcouples[i].Y, 0, 0});
	}
}


