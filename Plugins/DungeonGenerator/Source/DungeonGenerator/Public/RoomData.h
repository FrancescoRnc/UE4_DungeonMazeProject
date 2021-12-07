// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoomData.generated.h"


/**
* 
*/
enum class EWorldDirection : uint8
{
	NORTH = 1,
	EAST = 2,
	SOUTH = 4,
	WEST = 8
};

/**
* 
*/
struct FDoorInfo
{
	FName SourceRoomName;
	FName NextRoomName;	
	int32 SourceRoomIndex;
	int32 NextRoomIndex;
	EWorldDirection Direction;
};

/**
* 
*/
struct FRoomInfo
{
	FName RoomName;
	int32 IndexInGrid;
	FIntVector CoordinateInGrid;
	TArray<FDoorInfo> DoorsInfo;
	int32 PresetID;
	FName PresetName;
	FName PresetPath;
};


/**
 * 
 */
UCLASS()
class DUNGEONGENERATOR_API URoomPreset : public UObject
{
	GENERATED_BODY()

	public:

	URoomPreset();

	UPROPERTY(VisibleAnywhere)
	int32 PresetID = -1;
	
	UPROPERTY(EditAnywhere)
	FString RoomName;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WallsMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DoorsMesh;

	UFUNCTION(BlueprintCallable)
	void DoThings();
};
