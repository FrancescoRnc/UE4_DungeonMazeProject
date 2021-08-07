// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "DungeonBuilder.h"
#include "DungeonDoor.h"
#include "DungeonCrate.h"

#include "Engine/LevelStreamingDynamic.h"

#include "DungeonMaze.generated.h"




// Level Room Content Info Data Asset
UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API ULevelRoomContentInfo : public UDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LevelTypeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* EnvironmentMesh;

};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Structs


USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name = L"None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelRoomContentInfo* RoomAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector GridCoords = {0, 0, 0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D RoomSize = {4000.f, 4000.f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TMap<ERoomCardinals, FName> Doors;
	TArray<FDoorInfo> Doors;
	
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


class ARoom;

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

	UFUNCTION(BlueprintCallable)
	void GenerateRoom(ARoom* inRoom, const FRoomInfo& _info);
	
	UFUNCTION(BlueprintCallable)
	void SetEnvironment(UStaticMeshComponent* inComponent);
	UFUNCTION(BlueprintCallable)
	void GenerateDoors(const FRoomInfo& inInfo, TArray<ADungeonDoor*>& inDoors, UStaticMesh* mesh);
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

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	//ULevelRoomContentInfo* RoomAsset;

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

	public:
	
	UFUNCTION(BlueprintCallable)
	void GenerateRoom(const FRoomInfo& _info);
	
	UFUNCTION(BlueprintCallable)
	void GenerateDoors();
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// Dungeon Builder
UCLASS()
class DUNGEONMAZEPROJECT_API UCampainDungeonBuilder : public UObject, public IDungeonBuilder
{
	GENERATED_BODY()

	public:
	
	UCampainDungeonBuilder();	

	virtual void BuildDungeon_Implementation(const int sizeX, const int sizeY) override;
	//virtual void BuildRoom_Implementation(const FIntVector coords) override;
	virtual void BuildRooms_Implementation(ULevelRoomContentInfo* _asset, const TArray<FIntVector>& coords) override;
	//virtual void BuildDoor_Implementation(const FName& roomFrom, const FName& roomTo) override;
	virtual void BuildDoors_Implementation(UInteractableData* _asset, const TArray<int>& inGrid) override;

	UFUNCTION(BlueprintCallable)
	FDungeonInfo& GetDungeon();

	UFUNCTION(BlueprintCallable)
	TArray<int> GetGridValues(const int width, const int height);

	UFUNCTION(BlueprintCallable)
	TArray<FIntVector> GetRoomsCoordsByValues(const TArray<int>& inValues);

	UFUNCTION(BlueprintCallable)
	const FRoomInfo GetRoomByGridIndex(const TArray<FRoomInfo>& rooms, const int gridindex) const;

	UFUNCTION(BlueprintCallable)
	const FRoomInfo GetRoomByCoords(const TArray<FRoomInfo>& rooms, const int x, const int y) const;

	void BuildDoorsForRoom(const TArray<int>& inGrid, const int roomIndex);
	
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

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	//int dLevelIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridHeight;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	//ULevelRoomContentInfo* RoomAsset;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString FirstRoomLevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString GenRoomLevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	FString LastRoomLevelName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ARoom*> refRooms;
	
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//TArray<ULevelStreamingDynamic*> RoomInstances;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
    UCampainDungeonBuilder* Builder;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	//ULevelRoomContentInfo* RoomAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void SwitchRooms(FName newRoom);
	
	UFUNCTION(BlueprintCallable)
	void CreateDungeonInfo();

	UFUNCTION(BlueprintCallable)
	void GenerateDungeonFromInfo(const FDungeonInfo& dungeoninfo);

	//UFUNCTION(BlueprintCallable)
	//void SetLevelVisibility(const int index, const bool value);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDungeonInfo info;	
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 





