// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "DungeonMaze.h"

#include "DungeonGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMAZEPROJECT_API UDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:

	UDungeonGameInstance();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDungeonInfo sDungeonInfo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, FRoomInfo> sRoomsInfo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ADungeonMaze* refDungeon;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Saved Assets")
	TMap<FString, ULevelRoomContentInfo*> RoomAssets;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Saved Assets")
	TMap<FString, UInteractableData*> InteractabeAssets;

	UFUNCTION(BlueprintCallable)
	void SaveDungeonRoomsData(const FDungeonInfo& dungeon, const TArray<FRoomInfo>& rooms);
};
