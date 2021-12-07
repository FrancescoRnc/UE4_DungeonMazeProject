// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "DungeonUtilities.h"
#include "RoomData.h"
#include "DungeonRoom.generated.h"


/**
* 
*/
UCLASS(BlueprintType)
class DUNGEONGENERATOR_API ADungeonRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonRoom();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* FloorMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* WallsMeshComponent;
	
	FRoomInfo RoomInfo;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void Locate();

	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void Generate(const TSoftObjectPtr<URoomPreset> Preset);
	
	void InsertData(const FRoomInfo& Info);	
};
