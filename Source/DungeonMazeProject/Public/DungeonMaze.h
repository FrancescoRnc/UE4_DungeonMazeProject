// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "DungeonBuilder.h"
#include "DungeonDoor.h"
#include "DungeonCrate.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "DungeonMaze.generated.h"



/**
 * Level Room Content Info Data Asset,
 * which contains data about the exterior aspect,
 * and which kind of Room is.
 */
UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API ULevelRoomContentInfo : public UDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LevelTypeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D RoomSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* PlatformMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* WallMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UMaterialInterface* WallsMaterial;

};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Structs

/**
* Room Info struct, contains mandatory data about the Room to be generated:
* - FRoomInfo::Name: The name of the Room instance;
* - FRoomInfo:: RoomAsset: The Data Asset to be used:
* - FRoomInfo::GridCoords: The Coordinates, as Integers, referred to the Grid space;
* - FRoomInfo::RoomSize: The size of the current Room;
* - FRoomInfo::Doors: The list of Doors that bring to other Rooms.
*	This variable will be used to generate the Room's Doors.
*/
USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name = L"None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULevelRoomContentInfo* RoomAsset = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GridIndex = -1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector GridCoords = {0, 0, 0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D RoomSize = {4000.f, 4000.f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorInfo> Doors = {};
	
};

/**
* Dungeon Info struct, which contains data about the Dungeon to be generated:
* - FDungeonInfo::GridWidth/GridHeight: 2D size of the Grid space, which will be used to dispose all the Rooms of the Dungeon;
* - FDungeonInfo::RoomsCount: The total number of Rooms in the Dungeon:
* - FDungeonInfo::Rooms: The list of all the Rooms;
*/
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

/**
* New Level Instance Params struct: NOT USED FOR NOW!
*/
USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FNewLevelInstanceParams
{
	GENERATED_BODY()
	
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


/**
* Room base class: Abstaction of the class that represents the Room in the Level.
* This class contains functions that generate the Room itself, and contains data for its identity in the Level;
*/
UCLASS()
class DUNGEONMAZEPROJECT_API ARoom : public AActor
{
	GENERATED_BODY()

	public:

	ARoom();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* PlatformMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly) 
	UStaticMeshComponent* WallsMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UMaterialInterface* Material;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UMaterialInstanceDynamic* Transparency;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRoomInfo Info;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<ERoomCardinals, ADungeonDoor*> Doors;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<ADungeonDoor*, ARoom*> SiblingRooms;

	/**
	 * This function provides the generation of the Room into the Level, by setting data like Environment Mesh ecc.
	 * @param _info The data to be used for Room generation
	 */
	UFUNCTION(BlueprintCallable)
	void GenerateRoom(const FRoomInfo& _info);

	/**
	* This function provides the generation of the Doors contained by the Room.
	* @param _info The data to be used for Door generation
	*/
	UFUNCTION(BlueprintCallable)
	TArray<ADungeonDoor*> GenerateDoors(const TArray<FDoorInfo>& _info);

	DECLARE_EVENT(ARoom, FRoomGenerated)
	FRoomGenerated& OnRoomGenerated();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


/**
 *
 */
UCLASS()
class DUNGEONMAZEPROJECT_API ARoomLinker : public AActor
{
	GENERATED_BODY()

	public:

	ARoomLinker();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* DoorOne;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* DoorTwo;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* TriggerOne;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* TriggerTwo;

	// X -> First Room Grid Index
	// Y -> Second Room Grid Index
	// Z -> First Room Source Cardinal
	// W -> Second Room Source Cardinal
	FIntVector4 RoomsInfoPack;


	void LocateDoors(const FIntVector4 _roomsinfopack);


	UFUNCTION(BlueprintCallable)
	void OnFirstTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnSecondTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void GoToFirstRoom();

	UFUNCTION(BlueprintCallable)
	void GoToSecondRoom();
	
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


/**
* UDungeonInfoBuilder: Implementation of IDungeonBuilder.
* This class builds the Dungeon Info struct to be used for the real Dungeon generation.
* Here are some extra functions for this implementation class.
*/
UCLASS()
class DUNGEONMAZEPROJECT_API UDungeonInfoBuilder : public UObject, public IDungeonBuilder
{
	GENERATED_BODY()

	public:
	
	UDungeonInfoBuilder();	

	// Implementation from IDungeonBuilder
	virtual void BuildDungeon_Implementation(const int sizeX, const int sizeY) override;
	virtual void BuildRooms_Implementation(const TArray<ULevelRoomContentInfo*>& _assets, const TArray<int>& inGrid) override;
	virtual void BuildDoors_Implementation(UInteractableData* _asset, const TArray<int>& inGrid) override;

	/**
	 * This function returns the result of the Target Dungeon Object.
	 * @return The Dungeon Info to be used for Dungeon Generation.
	 */
	UFUNCTION(BlueprintCallable)
	FDungeonInfo& GetDungeon();

	/**
	 * This function returns the representation of a Grid used to generate the Dungeon and Rooms.
	 * @param width Width of the Grid.
	 * @param height Height of the Grid.
	 * @return An Array of integers that represents the Grid space.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<int> GetGridValues(const int width, const int height);

	/**
	* This function returns Coordinates to be given to Rooms.
	* @param inValues An array of Integers that represents the Grid.
	* @return An Array of Vectors, as Integers, that will be the Coordinates of the Rooms.
	*/
	UFUNCTION(BlueprintCallable)
	TArray<FIntVector> GetRoomsCoordsByValues(const TArray<int>& inValues);

	/**
	* This function returns Room Info by given the Index from the source Grid.
	* @attention This function is O(N), so this function will be optimized soon
	* @param rooms An Array of Room Info to be used for comparisons.
	* @param gridindex Index from a Grid.
	* @return A Room Info object.
	*/
	UFUNCTION(BlueprintCallable)
	const FRoomInfo GetRoomByGridIndex(const TArray<FRoomInfo>& rooms, const int gridindex) const;

	/**
	* This function returns Room Info by given some Coordinates, and compared with other Rooms.
	* @attention This function is O(N), so this function will be optimized soon
	* @param rooms An Array of Room Info to be used for comparisons.
	* @param x X Coordinate.
	* @param y Y Coordinate.
	* @return A Room Info Object.
	*/
	UFUNCTION(BlueprintCallable)
	const FRoomInfo GetRoomByCoords(const TArray<FRoomInfo>& rooms, const int x, const int y) const;

	
	private:
	
	UPROPERTY()
	FDungeonInfo outDungeon;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Dungeon Maze
/**
* Dungeon Maze base class: Abstraction of the class that represents the Dungeon itself in the Level;
* This class contains a Builder object that generates the Dungeon data struct, and some variables for Edit purpose;
* This class contains functions that generate Dungeon Info, and that generate the true Dungeon, with Rooms and Doors, in the Level;
*/
UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonMaze();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridHeight;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, ARoom*> refRooms;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ADungeonDoor*> refAllDoors;
	
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
    UDungeonInfoBuilder* Builder;

	/**
	 * This creates the Dungeon Info by using the Dungeon Builder.
	 */
	UFUNCTION(BlueprintCallable)
	void CreateDungeonInfo();

	/**
	* This generates the Dungeon itself by using the given Dungeon Info parameter.
	* @param dungeoninfo The Dungeon Info Object used to get data for Dungeon generation.
	*/
	UFUNCTION(BlueprintCallable)
	void GenerateDungeonFromInfo(const FDungeonInfo& dungeoninfo);

	UFUNCTION(BlueprintCallable)
	void BuildAndGenerate();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FDungeonInfo info;


	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

/**
 *
 */
UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()

	public:

	ADungeonGenerator();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UInstancedStaticMeshComponent* FloorInstances;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UInstancedStaticMeshComponent* WallInstances;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UInstancedStaticMeshComponent* DoorInstances;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridWidth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Edit")
	int GridHeight;


	UFUNCTION(BlueprintCallable)
	void Generate();

	UFUNCTION(BlueprintCallable)
	void LinkRooms(const TArray<FIntVector>& _inroomcouples);

	protected:
	virtual void BeginPlay() override;
};



