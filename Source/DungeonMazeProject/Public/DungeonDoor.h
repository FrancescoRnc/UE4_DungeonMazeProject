// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"


#include "DungeonDoor.generated.h"


UENUM(BlueprintType)
enum class ERoomCardinals : uint8 { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3  };


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

	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//UInteractableData* DoorData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FDoorInfo Info;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* BoxInteractionColision;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	FName CurrentLevelName;
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
	FName NextLevelName;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//UInteractableData Data;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;


	void Initialize(const FDoorInfo& _info);
};
