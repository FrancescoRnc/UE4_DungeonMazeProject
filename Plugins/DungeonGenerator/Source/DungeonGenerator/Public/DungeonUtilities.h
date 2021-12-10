// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomData.h"


using URoomPresetPtr = class URoomPreset*;
//using URoomPresetPtr = TSoftObjectPtr<class URoomPreset>;
using URoomPresetRef = TSharedRef<URoomPreset>;


/**
* IDungeonBuilder Interface
*/
class IDungeonBuilder
{
public:
	//virtual ~IDungeonBuilder() = 0;

	virtual void BuildDungeon() = 0;
	virtual void BuildRooms() = 0;
	virtual void BuildDoors() = 0;
	
};


/**
 * 
 */
class FDungeonUtils
{
public:	
	FDungeonUtils();	

	const FName PrefabricsPath = TEXT("/Game/Prefabrics");
	const FName DungeonDataPath = TEXT("/Game/DungeonData");

	TMap<int32, URoomPresetPtr> RoomPresetsMap;
	TArray<URoomPresetPtr> RoomPresets;
	TArray<FName> RoomPresetPaths;
	URoomPresetPtr SelectedPreset;

	class UDungeonData* CurrentDungeonFile;
	FName CurrentDungeonPath;
	UPackage* DungeonPackage;
	

	void GetPresetsOnLoad();
	void AddAssetReference(URoomPresetPtr NewPreset);
	void DeleteAssetReference(URoomPresetPtr NewPreset);

	
	const bool RescanAssetReferences() const;
	bool RGCommandMakeRoom(const FString AssetName);
	UDungeonData* SaveNewDungeonData(const struct FDungeonInfo& Info);
	UDungeonData* GetDungeonDataAsset();
	
	// Obsolete	
	void CheckAssetReference(const FAssetData& Asset);
	// Obsolete
	void RemoveAssetReference(const FAssetData& Asset);	

	URoomPresetPtr GetRoomPresetByName(const FName Name) const;
	URoomPresetPtr GetRoomPresetByPath(const FName Path) const;
	
	FORCEINLINE URoomPresetPtr GetRoomPresetByID(const int32 ID) const
	{
		return RoomPresetsMap[ID];
	}
	
	FORCEINLINE TArray<URoomPresetPtr> GetRoomPresets() const
	{
		return RoomPresets;
	}

	FORCEINLINE URoomPresetPtr GetSelectedPreset() const
	{
		return SelectedPreset;
	}

	FORCEINLINE int32 GetPresetsCount() const
	{
		if (RoomPresets.Num() != RoomPresetsMap.Num())
		{
			UE_LOG(LogTemp, Fatal, TEXT("ERROR: Presets Count between Array and map containers is not equal!"));
		}
		return RoomPresets.Num();
	}
	

private:	
	
	TMap<FName, UPackage*> PresetPackages;
	

public:
	FORCEINLINE static FDungeonUtils* Get()
	{
		return Instance;
	}

private:	
	static FDungeonUtils* Instance;
	
};
