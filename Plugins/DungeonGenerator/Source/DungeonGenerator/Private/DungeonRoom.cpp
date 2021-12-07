// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonRoom.h"
#include "DungeonGenerator.h"

// Sets default values
ADungeonRoom::ADungeonRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	FloorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	WallsMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Walls Mesh"));

	FloorMeshComponent->SetupAttachment(RootComponent);
	WallsMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADungeonRoom::BeginPlay()
{
	Super::BeginPlay();

	//Generate();
}

// Called every frame
//void ADungeonRoom::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ADungeonRoom::Locate()
{
	const FVector Location = FVector(
    	
    	RoomInfo.CoordinateInGrid.Y * 5000.f,
    	-RoomInfo.CoordinateInGrid.X * 5000.f,
    	0);
    SetActorLocation(Location);
}

void ADungeonRoom::Show()
{
	const FSoftObjectPath Path(RoomInfo.PresetPath);
	const TSoftObjectPtr<URoomPreset> Preset(Path);
	
	FloorMeshComponent->SetStaticMesh(Preset->FloorMesh);
	WallsMeshComponent->SetStaticMesh(Preset->WallsMesh);
}

void ADungeonRoom::Generate(const TSoftObjectPtr<URoomPreset> Preset)
{
	//URoomPreset* GetPreset =
	//	FDungeonUtils::Get()->GetRoomPresetByName(RoomInfo.PresetName);
	FloorMeshComponent->SetStaticMesh(Preset->FloorMesh);
	WallsMeshComponent->SetStaticMesh(Preset->WallsMesh);
}

void ADungeonRoom::InsertData(const FRoomInfo& Info)
{
	RoomInfo = Info;
	//RoomPreset = *Info.Preset.Get();	
}