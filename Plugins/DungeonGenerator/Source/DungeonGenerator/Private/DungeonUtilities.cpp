// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"


// FDungeonUtils
FDungeonUtils* FDungeonUtils::Instance = nullptr;

FDungeonUtils::FDungeonUtils()
{
	Instance = this;
}

void FDungeonUtils::GetPresetsOnLoad()
{
	RoomPresets.Empty();
	
	TArray<FAssetData> Assets;
	FAssetRegistryModule::GetRegistry().GetAssetsByPath(PrefabricsPath, Assets);
	for (FAssetData Asset : Assets)
	{
		URoomPresetPtr NewPreset = Cast<URoomPreset>(Asset.GetAsset());
		AddAssetReference(NewPreset);		
	}

	const bool Rescan = RescanAssetReferences();
	UE_LOG(LogTemp, Warning, TEXT("Asset References Got? %d"), Rescan);
}

void FDungeonUtils::AddAssetReference(URoomPresetPtr NewPreset)
{	
	NewPreset->PresetID = FMath::Rand();
	RoomPresets.Add(NewPreset);
	RoomPresetsMap.Add(NewPreset->PresetID, NewPreset);
}

void FDungeonUtils::DeleteAssetReference(URoomPresetPtr NewPreset)
{
	if (RoomPresets.Contains(NewPreset) && RoomPresetsMap.Contains(NewPreset->PresetID))
	{
		RoomPresets.Remove(NewPreset);
		RoomPresetsMap.Remove(NewPreset->PresetID);
	}
}

const bool FDungeonUtils::RescanAssetReferences() const
{
	TArray<FAssetData> Assets;
	if (FAssetRegistryModule::GetRegistry().GetAssetsByPath(PrefabricsPath, Assets, true))
	{
		for (const FAssetData& Asset : Assets)
		{
			if (const URoomPresetPtr Preset = Cast<URoomPreset>(Asset.GetAsset()))
			{
				if (!RoomPresets.Contains(Preset))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void FDungeonUtils::CheckAssetReference(const FAssetData& Asset)
{
	if (Asset.PackagePath == PrefabricsPath)
	{
		const URoomPresetPtr NewPreset = Cast<URoomPreset>(Asset.GetAsset());
		if (NewPreset && !RoomPresets.Contains(NewPreset))
		{
			AddAssetReference(NewPreset);
		}
	}	
}

void FDungeonUtils::RemoveAssetReference(const FAssetData& Asset)
{
	if (Asset.PackagePath == PrefabricsPath)
	{
		const URoomPresetPtr Object = Cast<URoomPreset>(Asset.GetAsset());
		if (RoomPresets.Contains(Object))
		{
			RoomPresets.Remove(Object);
			RoomPresetsMap.Remove(Object->PresetID);
		}
	}
}

URoomPresetPtr FDungeonUtils::GetRoomPresetByName(const FName Name) const
{
	for (const URoomPresetPtr Preset : RoomPresets)
	{
		if (Preset->GetFName() == Name)
		{
			return Preset;
		}
	}
	return nullptr;
}

URoomPresetPtr FDungeonUtils::GetRoomPresetByPath(const FName Path) const
{
	const FAssetData Asset = FAssetRegistryModule::GetRegistry().GetAssetByObjectPath(Path);
	if (Asset.IsValid())
	{
		if (const URoomPresetPtr Preset = Cast<URoomPreset>(Asset.GetAsset()))
		{
			return Preset;
		}
	}		
	return nullptr;
}

bool FDungeonUtils::RGCommandMakeRoom(const FString AssetName)
{
	if (AssetName == "")
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING NAME FOR NEW ROOM ASSET"));
		return false;
	}
	if (RoomPresets.Contains(GetRoomPresetByName(*AssetName)))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASSET OF NAME %s ALREADY CREATED. PLEASE TRY WITH A DIFFERENT NAME"), *AssetName);
		return false;
	}
	const FString Path = PrefabricsPath.ToString();
	const FString PackageName = FString::Printf(TEXT("%s_Pkg"), *AssetName);
	const FString PackagePath = FString::Printf(TEXT("%s/%s"), *Path, *PackageName);
	const FString FullPathName = FString::Printf(TEXT("%s.%s"), *PackagePath, *AssetName);

	UPackage* Package = CreatePackage(*PackagePath); 
	URoomPreset* NewRoom = NewObject<URoomPreset>(Package, *AssetName, RF_Public | RF_Standalone);
	//NewRoom->AddToRoot();
	
	AddAssetReference(NewRoom);
	Packages.Add(*PackageName, Package);

	const FString Filename = FPackageName::LongPackageNameToFilename(
		Package->GetName(), FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, NewRoom, RF_Public | RF_Standalone, *Filename);
	FAssetRegistryModule::AssetCreated(NewRoom);

	Package->MarkPackageDirty();

	return true;
}
