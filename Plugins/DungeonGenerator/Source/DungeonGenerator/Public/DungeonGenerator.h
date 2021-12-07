// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DungeonData.h"
#include "DungeonUtilities.h"
#include "Modules/ModuleManager.h"





/**
* FRoomGenerator Class
*/
class FRoomGenerator
{
public:
	FRoomGenerator();

	FReply DGCommandRoomPreview();
	
	class ADungeonRoom* Generate(const FRoomInfo& Info);
};


/**
* FDungeonGenerator CLass
*/
class FDungeonGenerator : public IDungeonBuilder
{
public:
	FDungeonGenerator();
	//virtual ~FDungeonGenerator() override;

	/** IDungeonBuilder implementation */
	virtual void BuildDungeon() override;
	virtual void BuildRooms(TArray<FRoomInfo>& OutRoomsInfo) override;
	virtual void BuildDoors(TArray<FDoorInfo>& OutDoorsInfo) override;
	
	FORCEINLINE const FDungeonInfo& GetDungeonInfo() const
	{
		return OutDungeonInfo;
	}

private:
	FDungeonInfo OutDungeonInfo;
};


/**
* FDungeonGeneratorModule Module Class
*/
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
	const FName DungeonGeneratorTabName = TEXT("Dungeon Generator");
    const int32 DGSpacerSize = 10;
    const int32 HPadding = 10;
    const int32 VPadding = 10;
	
	TSharedPtr<FDungeonGenerator> DungeonGenerator;
	TSharedPtr<FRoomGenerator> RoomGenerator;
	TSharedPtr<FDungeonUtils> DungeonUtils;
	
	FDungeonInfo CurrentDungeonInfo;
	TArray<FRoomInfo> RoomsInfo;
	TArray<FDoorInfo> DoorsInfo;
	
	
	FReply DGCommandGenerate();
    FReply DGCommandSave();
	FReply DGCommandReset();
	FReply DGCommandPreview();
	FReply DGCommandRoomPreview();
	
	void RGCommandMakeRoom(const FText& InText, ETextCommit::Type InCommitType);

};
