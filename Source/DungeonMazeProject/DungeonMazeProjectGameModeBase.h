// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "DungeonMazeProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonMazeProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	public:

	ADungeonMazeProjectGameModeBase();

	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TMap<FName, ULevelRoomContentInfo> RoomsInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName CurrentRoomName;
	
	

	virtual void BeginPlay() override;


	void LoadNewRoom(const FName newRoomName);
};
