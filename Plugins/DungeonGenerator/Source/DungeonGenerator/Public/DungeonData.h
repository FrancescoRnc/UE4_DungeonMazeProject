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
};


/**
 * 
 */
UCLASS()
class DUNGEONGENERATOR_API UDungeonData : public UObject
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere)
	FIntVector GridSize;

	// ESTENDI PER SERIALIZZAZIONE

	UPROPERTY(VisibleAnywhere)
	TArray<int32> RoomsPresetIDs;
	
	UPROPERTY(EditAnywhere)
	TArray<FVector> RoomsLocations;

	UPROPERTY(EditAnywhere)
	TArray<FVector> DoorsLocations;
};
