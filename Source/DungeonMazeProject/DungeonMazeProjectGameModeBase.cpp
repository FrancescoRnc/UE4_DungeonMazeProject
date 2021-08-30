// Copyright Epic Games, Inc. All Rights Reserved.


#include "DungeonMazeProjectGameModeBase.h"

#include "GameFramework/Character.h"


ADungeonMazeProjectGameModeBase::ADungeonMazeProjectGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = false;

	
}

void ADungeonMazeProjectGameModeBase::ChangeCharacterLocation(ACharacter* _character, const FTransform& _refTransform)
{
	auto _loc =	_refTransform.GetLocation() +
						(_refTransform.GetRotation().GetForwardVector() * 100.f) +
						FVector(0, 0, 200);
	auto _rot = _refTransform.GetRotation().Inverse();
	_character->SetActorLocationAndRotation(_loc, _rot);
}

void ADungeonMazeProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	
}

