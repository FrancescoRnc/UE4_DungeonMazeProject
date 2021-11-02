// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "DungeonBuilderInterface.h"
#include "RoomData.h"
#include "Modules/ModuleManager.h"


static const int32 DGSlateSpacerSize = 10;


// Structs
enum class EWorldDirection : uint8 { NORTH = 0, EAST = 1, WEST = 2, SOUTH = 3, MAX = 4 };

struct FDoorInfo
{
	FName SourceRoomName;
	FName NextRoomName;
	EWorldDirection Direction;

};

struct FRoomInfo
{
	FName RoomName;
	FIntVector IndexInGrid;
	TArray<FDoorInfo> DoorsInfo;

};

struct FDungeonInfo
{
	TArray<FRoomInfo> RoomsInfo;
	
};


// IDungeonBuilder Interface
class IDungeonBuilder
{
	public:
	//virtual ~IDungeonBuilder() = 0;

	virtual void BuildDungeon() = 0;
	virtual void BuildRooms(TArray<FRoomInfo>& OutRoomsInfo) = 0;
	virtual void BuildDoors(TArray<FDoorInfo>& OutDoorsInfo) = 0;
	
};


// FRoomGenerator Class
class FRoomGenerator
{
	public:

	FRoomGenerator();

	FName PrefabricsPath = TEXT("/Game/Prefabrics");

	FORCEINLINE const class URoomData* GetRoomData(FName Name) const
	{
		for (const URoomData* Data : RoomDataAssets)
		{
			if (Data->GetFName() == Name)
			{
				return Data;
			}
		}
		return nullptr;
	}

	FORCEINLINE int32 GetAssetNum() const
	{
		return RoomDataAssets.Num();
	}

	void RescanAssetReferences();
	void CheckAssetReference(const FAssetData& Asset);
	void RemoveAssetReference(const FAssetData& Asset);
	bool RGCommandMakeRoom(const FString AssetName);

	
	//TMap<FName, URoomData*> RoomDataAssets;
	TArray<URoomData*> RoomDataAssets;

private:
	TMap<FName, UPackage*> Packages;


};


// FDungeonGenerator Class
class FDungeonGenerator : public IDungeonBuilder
{
public:

	FDungeonGenerator();
	//virtual ~FDungeonGenerator() override;

	/** IDungeonBuilder implementation */
	virtual void BuildDungeon() override;
	virtual void BuildRooms(TArray<FRoomInfo>& OutRoomsInfo) override;
	virtual void BuildDoors(TArray<FDoorInfo>& OutDoorsInfo) override;
	//virtual void BuildDungeon_Implementation() override;
	//virtual void BuildRooms_Implementation() override;
	//virtual void BuildDoors_Implementation() override;

	const FDungeonInfo& GetDungeonInfo() const;


private:

	FDungeonInfo OutDungeonInfo;
};


// FDungeonGeneratorModule Class
class FDungeonGeneratorModule : public IModuleInterface, public FSelfRegisteringExec
{
public:	

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** FSelfRegisteringExec implementation */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	TSharedRef<class SDockTab> SpawnNomadTab(const FSpawnTabArgs& TabSpawnArgs);


private:
	TSharedPtr<FDungeonGenerator> DungeonGenerator;
	TSharedPtr<class FRoomGenerator> RoomGenerator;
	FName DungeonGeneratorTabName = TEXT("Dungeon Generator");

	FReply DGCommandGenerate();
    FReply DGCommandSave();
	FReply DGCommandPreview();

	void RGCommandMakeRoom(const FText& InText, ETextCommit::Type inCommitType);
	
};
