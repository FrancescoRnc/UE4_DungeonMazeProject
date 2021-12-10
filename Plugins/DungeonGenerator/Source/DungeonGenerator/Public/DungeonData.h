// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonSchemeMaker.h"
#include "UObject/NoExportTypes.h"
#include "DungeonData.generated.h"


/**
*  FDungeonInfo Struct
*/
struct FDungeonInfo
{
	FGrid Grid;
	FIntVector GridSize;
	TArray<int32> GridScheme;
	
	TArray<struct FRoomInfo> RoomsInfo;

	void Reset();
};


/**
 * Object File used for In-Level generation of a new Dungeon
 * - GridSize: The full 2D size (Z is not used) of the stored Grid;
 * - PathLength: The length of the Path expressed by the Rooms count;
 * - GridScheme: A 1D representation of our Grid, in which any non-zero number stands for its Cell Pattern;
 * - RoomsPresetID: An Array of Indices held to track the corresponding path of the Paths' Array;
 * - RoomsCoordinate: ;
 * - DoorsSourceRoomIndex: ;
 * - DoorsNextRoomIndex: ;
 * - DoorsDirection: ;
 */
UCLASS()
class DUNGEONGENERATOR_API UDungeonData : public UObject
{
	GENERATED_BODY()

	public:

	// Grid Information
	UPROPERTY(VisibleAnywhere)
	FIntVector GridSize;

	UPROPERTY(VisibleAnywhere)
	int32 PathLength;	

	UPROPERTY(VisibleAnywhere)
	TArray<int32> GridScheme;
	
	
	// Rooms Information
	UPROPERTY(VisibleAnywhere)
	TArray<int32> RoomsPresetID;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> RoomsPresetPaths;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> RoomsCoordinate;

	UPROPERTY(VisibleAnywhere)
	TArray<int32> RoomsGridIndex;

	
	// Doors Information	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> DoorsSourceRoomIndex;
	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> DoorsNextRoomIndex;
	
	UPROPERTY(VisibleAnywhere)
	TArray<int32> DoorsDirection;

	
	UFUNCTION(BlueprintCallable)
	void Reset();
};
