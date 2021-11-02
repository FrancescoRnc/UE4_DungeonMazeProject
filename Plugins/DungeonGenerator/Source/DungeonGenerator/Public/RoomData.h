// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RoomData.generated.h"


/**
 * 
 */
UCLASS()
class DUNGEONGENERATOR_API URoomData : public UObject
{
	GENERATED_BODY()

	public:

	URoomData();
	
	UPROPERTY(EditAnywhere)
	FString RoomName;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* PlatformMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WallsMesh;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DoorsMesh;
};
