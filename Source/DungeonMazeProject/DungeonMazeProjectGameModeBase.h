// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
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

	UFUNCTION(BlueprintCallable)
	void ChangeCharacterLocation(ACharacter* _character, const FTransform& _refTransform);
	
	protected:
	virtual void BeginPlay() override;

	
	
};
