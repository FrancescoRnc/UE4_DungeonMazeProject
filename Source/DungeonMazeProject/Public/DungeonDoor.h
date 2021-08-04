// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"


#include "DungeonDoor.generated.h"

UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonDoor : public AActor, public IInteractable
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonDoor();

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
};
