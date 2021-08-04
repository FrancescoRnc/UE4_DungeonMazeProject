// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DungeonBuilder.h"
#include "DungeonDoor.h"
#include "DungeonCrate.h"

#include "Engine/LevelStreamingDynamic.h"

#include "DungeonMaze.generated.h"

UENUM(BlueprintType)
enum class RoomCardinals : uint8 { NORTH = 0, WEST = 1, EAST = 2, SOUTH = 3 };


// Level Room Content Info Data Asset
UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API ULevelRoomContentInfo : public UDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* EnvironmentMesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInteractableData* NorthDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInteractableData* SouthDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInteractableData* EastDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UInteractableData* WestDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UInteractableData*> Crates;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool RandomizeEnemies;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UInteractableData*> Enemies; // PROVVISORIO DO NOT USE

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UInteractableData*> OtherInteractables;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Structs
USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector GridCoords;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<RoomCardinals, FName> Doors;
	
};

USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FDungeonInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GridHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int RoomsCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FRoomInfo> Rooms;
};

USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FNewLevelInstanceParams
{
	GENERATED_BODY()

	//FNewLevelInstanceParams();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString OptionalLevelNameOverride = L"";
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Room Generator 
UCLASS()
class DUNGEONMAZEPROJECT_API URoomGenerator : public UObject
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	URoomGenerator();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelRoomContentInfo* RoomAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* EnvironmentMesh;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADungeonDoor* NorthDoor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADungeonDoor* SouthDoor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADungeonDoor* EastDoor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ADungeonDoor* WestDoor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ADungeonCrate*> Crates;

	UFUNCTION(BlueprintCallable)
	void SetEnvironment(UStaticMeshComponent* inComponent);
	UFUNCTION(BlueprintCallable)
	void BuildDoors(TArray<ADungeonDoor*> inDoors);
	UFUNCTION(BlueprintCallable)
	void BuildOtherActors();	
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Room
UCLASS()
class DUNGEONMAZEPROJECT_API ARoom : public AActor
{
	GENERATED_BODY()

	public:

	ARoom();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	ULevelRoomContentInfo* RoomAsset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	URoomGenerator* RoomGenerator;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UStaticMeshComponent* EnvironmentMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRoomInfo Info;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ADungeonDoor*> Doors;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ARoom*> SiblingRooms;

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// Dungeon Builder
UCLASS()
class DUNGEONMAZEPROJECT_API UCampainDungeonBuilder : public UObject, public IDungeonBuilder
{
	GENERATED_BODY()

	public:
	
	UCampainDungeonBuilder();	

	virtual void BuildDungeon_Implementation(int sizeX, int sizeY) override;
	virtual void BuildRoom_Implementation(FIntVector coords) override;
	virtual void BuildDoor_Implementation(FName roomFrom, FName roomTo) override;

	UFUNCTION(BlueprintCallable)
	FDungeonInfo& GetDungeon();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetGridValues(int width, int height);

	UFUNCTION(BlueprintCallable)
	TArray<FIntVector> GetRoomsCoordsByValues(TArray<int> inValues);
	
	private:
	
	UPROPERTY()
	FDungeonInfo outDungeon;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Dungeon Maze
UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonMaze();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	int dLevelIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridHeight;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString FirstRoomLevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString GenRoomLevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString LastRoomLevelName;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ULevelStreamingDynamic*> RoomInstances;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    UCampainDungeonBuilder* Builder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void SwitchRooms(FName newRoom);
	
	UFUNCTION(BlueprintCallable)
	void CreateDungeonInfo(UCampainDungeonBuilder* _builder);

	UFUNCTION(BlueprintCallable)
	void GenerateDungeonFromInfo(FDungeonInfo dungeoninfo);

	UFUNCTION(BlueprintCallable)
	void SetLevelVisibility(int index, bool value);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDungeonInfo info;	
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 





