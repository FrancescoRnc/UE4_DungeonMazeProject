// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InteractableObject.h"

#include "DungeonBuilder.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDungeonBuilder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONMAZEPROJECT_API IDungeonBuilder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BuildDungeon(int sizeX, int sizeY);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BuildRoom(FIntVector coords);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BuildDoor(FName roomFrom, FName roomTo);
};
