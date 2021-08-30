// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"


#include "DungeonDoor.generated.h"


UENUM(BlueprintType)
enum class ERoomCardinals : uint8 { NORTH = 0, EAST = 1, WEST = 2, SOUTH = 3, MAX = 4  };


USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FDoorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInteractableData* DoorAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERoomCardinals Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RoomOriginName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RoomDestinationName;
	
};


UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonDoor : public AActor, public IInteractable
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonDoor();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDoorInfo Info;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* BoxInteractionCollision;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FName CurrentLevelName;
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FName NextLevelName;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	float DistanceFromDoor;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	ADungeonDoor* LinkedDoor;

	FInteractionDelegate InteractDelegate;

	
	UFUNCTION(BlueprintCallable)
    void Initialize(const FDoorInfo& _info);

	UFUNCTION(BlueprintCallable)
	void GoToRoom(ACharacter* character);

	virtual void Interact_Implementation() override;
	
	
	protected:
    
	virtual void BeginPlay() override;
};
