// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGenerator.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/LayerManager/SLayerManager.h"
#include "Widgets/Layout/SSpacer.h"
#include "RoomData.h"
#include "SRoomSelectorCombo.h"
#include "Slate/Private/Widgets/Views/SListPanel.h"

#define LOCTEXT_NAMESPACE "FDungeonGeneratorModule"


FRoomGenerator::FRoomGenerator()
{

}

void FRoomGenerator::RescanAssetReferences()
{
	RoomDataAssets.Empty();

	TArray<FAssetData> Assets;
	if (FAssetRegistryModule::GetRegistry().GetAssetsByPath(PrefabricsPath, Assets, true))
	{
		for (const FAssetData& Asset : Assets)
		{
			if (URoomData* RoomData = Cast<URoomData>(Asset.GetAsset()))
			{
				RoomDataAssets.Add(RoomData);
			}
		}
	}
}

bool FRoomGenerator::RGCommandMakeRoom(const FString AssetName)
{
	if (AssetName == "")
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING NAME FOR NEW ROOM ASSET"));
		return false;
	}
	if (RoomDataAssets.Contains(GetRoomData(*AssetName)))
	{
		UE_LOG(LogTemp, Warning, TEXT("ASSET OF NAME %s ALREADY CREATED. PLEASE TRY WITH A DIFFERENT NAME"), *AssetName);
		return false;
	}
	const FString Path = PrefabricsPath.ToString();
	const FString PackageName = FString::Printf(TEXT("%s_Pkg"), *AssetName);
	const FString PackagePath = FString::Printf(TEXT("%s/%s"), *Path, *PackageName);
	const FString FullPathName = FString::Printf(TEXT("%s.%s"), *PackagePath, *AssetName);

	UPackage* Package = CreatePackage(*PackagePath);
	URoomData* NewRoom = NewObject<URoomData>(Package, *AssetName, RF_Public | RF_Standalone);
	//NewRoom->AddToRoot();

	RoomDataAssets.Add(NewRoom);

	FString Filename = FPackageName::LongPackageNameToFilename(Package->GetName(),
															   FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, NewRoom, RF_Public | RF_Standalone, *Filename);
	FAssetRegistryModule::AssetCreated(NewRoom);

	Package->MarkPackageDirty();

	return true;
}


void FRoomGenerator::CheckAssetReference(const FAssetData& Asset)
{
	if (Asset.PackagePath == PrefabricsPath)
	{
		URoomData* Object = Cast<URoomData>(Asset.GetAsset());
		if (Object)
		{
			FName ObjectName = Object->GetFName();
            //if (!RoomDataAssets.Contains(ObjectName))
			if (!RoomDataAssets.Contains(Object))
            {
            	RoomDataAssets.Add(Object);
            }
		}
        
	}	
}

void FRoomGenerator::RemoveAssetReference(const FAssetData& Asset)
{
	if (Asset.PackagePath == PrefabricsPath)
	{
		URoomData* Object = Cast<URoomData>(Asset.GetAsset());
		FName ObjectName = Object->GetFName();
		//if (RoomDataAssets.Contains(ObjectName))
		if (RoomDataAssets.Contains(Object))
		{
			//RoomDataAssets.Remove(ObjectName);
			RoomDataAssets.Remove(Object);
		}
		
	}
}

// FDungeonGenerator
FDungeonGenerator::FDungeonGenerator()
{
	
}

/*FDungeonGenerator::~FDungeonGenerator()
{
	
}*/

void FDungeonGenerator::BuildDungeon()
{
	OutDungeonInfo = {};
	UE_LOG(LogTemp, Display, TEXT("Building Dungeon..."));
}

void FDungeonGenerator::BuildRooms(TArray<FRoomInfo>& OutRoomsInfo)
{
	UE_LOG(LogTemp, Display, TEXT("Building Rooms..."));
}

void FDungeonGenerator::BuildDoors(TArray<FDoorInfo>& OutDoorsInfo)
{
	UE_LOG(LogTemp, Display, TEXT("Building Doors..."));
}

const FDungeonInfo& FDungeonGenerator::GetDungeonInfo() const
{
	return OutDungeonInfo;
}


// FDungeonGeneratorModule
void FDungeonGeneratorModule::StartupModule()
{
	DungeonGenerator = MakeShared<FDungeonGenerator>();
	RoomGenerator = MakeShared<FRoomGenerator>();
	RoomGenerator->RescanAssetReferences();

	FAssetRegistryModule::GetRegistry().OnAssetAdded()
		.AddRaw(RoomGenerator.Get(), &FRoomGenerator::CheckAssetReference);
	FAssetRegistryModule::GetRegistry().OnAssetRemoved()
		.AddRaw(RoomGenerator.Get(), &FRoomGenerator::RemoveAssetReference);
	
	const TSharedPtr<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	FTabSpawnerEntry SpawnerEntry = TabManager->RegisterNomadTabSpawner(DungeonGeneratorTabName,
		FOnSpawnTab::CreateRaw(this, &FDungeonGeneratorModule::SpawnNomadTab));
	//TabManager->TryInvokeTab(DungeonGeneratorTabName);
	
	UE_LOG(LogTemp, Display, TEXT("Dungeon Generator Startup"));
}

void FDungeonGeneratorModule::ShutdownModule()
{
	DungeonGenerator.Reset();

	const TSharedPtr<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	TabManager->UnregisterNomadTabSpawner(DungeonGeneratorTabName);
	
	UE_LOG(LogTemp, Display, TEXT("Dungeon Generator Shutdown"));
}

bool FDungeonGeneratorModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	const FString CmdAsString = Cmd;

	/*if (CmdAsString == "generate")
	{
		TArray<FRoomInfo> OutRoomsInfo;
		TArray<FDoorInfo> OutDoorsInfo;
		FDungeonInfo DungeonInfo;
		
		DungeonGenerator->BuildDungeon();
		DungeonGenerator->BuildRooms(OutRoomsInfo);
		DungeonGenerator->BuildDoors(OutDoorsInfo);
		DungeonInfo = DungeonGenerator->GetDungeonInfo();		

		UE_LOG(LogTemp, Display, TEXT("A NEW MAZE HAS BEEN GENERATED!"));
		return true;
	}
	else if (CmdAsString == "save")
	{
		
		
		UE_LOG(LogTemp, Display, TEXT("CURRENT MAZE HAS BEEN SAVED!"));
		return true;
	}
	else if (CmdAsString == "reset")
	{
		
		
		UE_LOG(LogTemp, Warning, TEXT("CURRENT MAZE HAS BEEN RESET!"));
		return true;
	}*/
	 if (CmdAsString == "queryfiles")
	{
		UE_LOG(LogTemp, Display, TEXT("Number of Asset Files: %d | Path: %s"),
			RoomGenerator->GetAssetNum(), *RoomGenerator->PrefabricsPath.ToString());
		for (const URoomData* Data : RoomGenerator->RoomDataAssets)
		{
			UE_LOG(LogTemp, Display, TEXT("Asset File: %s"), *Data->GetName());
		}		
		
		return true;
	}

	return false;
}

FReply FDungeonGeneratorModule::DGCommandGenerate()
{
	TArray<FRoomInfo> OutRoomsInfo;
	TArray<FDoorInfo> OutDoorsInfo;
	FDungeonInfo DungeonInfo;
		
	DungeonGenerator->BuildDungeon();
	DungeonGenerator->BuildRooms(OutRoomsInfo);
	DungeonGenerator->BuildDoors(OutDoorsInfo);
	DungeonInfo = DungeonGenerator->GetDungeonInfo();		

	UE_LOG(LogTemp, Display, TEXT("A NEW MAZE HAS BEEN GENERATED!"));
	
	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandSave()
{
	// Save Operations
	
	UE_LOG(LogTemp, Display, TEXT("CURRENT MAZE HAS BEEN SAVED!"));

	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandPreview()
{
	UE_LOG(LogTemp, Display, TEXT("HERE'S A PREVIEW OF THE MAP!"));
	
	return FReply::Handled();
}


void FDungeonGeneratorModule::RGCommandMakeRoom(const FText& InText, ETextCommit::Type inCommitType)
{
	if (inCommitType == ETextCommit::OnEnter)
	{
		RoomGenerator->RGCommandMakeRoom(InText.ToString());
	}
}


TSharedRef<SDockTab> FDungeonGeneratorModule::SpawnNomadTab(const FSpawnTabArgs& TabSpawnArgs)
{
	//TSharedRef<SRoomSelectorCombo> Selector = SNew(SRoomSelectorCombo).Source(&RoomGenerator->RoomDataAssets);
	//Selector->Options = &RoomGenerator->RoomDataAssets;
	
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot().AutoWidth().Padding(10, 0, 10, 0)
		[
			SNew(SBox)
            .HAlign(HAlign_Left).MaxDesiredWidth(1000)//.WidthOverride(500)
            .VAlign(VAlign_Top).MaxDesiredHeight(400)//.HeightOverride(100)
            [
            	SNew(SVerticalBox)
            	+SVerticalBox::Slot().AutoHeight().Padding(20,10,0,0)//.HAlign(HAlign_Left)
            	[
            		SNew(STextBlock).Text(LOCTEXT("DungeonCommandsTitle", "Dungeon Generator Commands:"))
            	]
            	+SVerticalBox::Slot()
            	[
            		SNew(SSpacer).Size(DGSlateSpacerSize)
            	]
            	+SVerticalBox::Slot().AutoHeight().Padding(20,0,0,0)//.HAlign(HAlign_Left)
            	[
            		SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("GenerateNewDungeon", "Generate New Dungeon"))
            		.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandGenerate)
            	]
            	+SVerticalBox::Slot()
            	[
            		SNew(SSpacer).Size(DGSlateSpacerSize)
            	]
            	+SVerticalBox::Slot().AutoHeight().Padding(20,0,0,0)//.HAlign(HAlign_Left)
            	[
            		SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("SaveCurrentDungeon", "Save Current Dungeon"))
            		.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandSave)
            	]
            	+SVerticalBox::Slot()
				[
					SNew(SSpacer).Size(DGSlateSpacerSize)
				]
				+SVerticalBox::Slot().AutoHeight().Padding(20,0,0,0)//.HAlign(HAlign_Left)
				[
					SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("Dungeon Preview", "Show Preview"))
					.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandPreview)
				]
				+SVerticalBox::Slot()
				[
					SNew(SSpacer).Size(DGSlateSpacerSize)
				]
				+SVerticalBox::Slot().AutoHeight().Padding(20,0,0,0)//.HAlign(HAlign_Left)
				[
					SNew(SRoomSelectorCombo<URoomData*>).Source(&RoomGenerator->RoomDataAssets)
					/*SNew(SComboBox<URoomData*>)
					.OptionsSource(&RoomGenerator->RoomDataAssets)
					.OnGenerateWidget_Lambda([this](URoomData* InOption) -> TSharedRef<SWidget>
					{
						return SNew(STextBlock).Text(FText::FromString(*InOption->GetName()));
					})
					[
						SNew(STextBlock).Text(LOCTEXT("None", "None"))
						//.Text_Lambda([this]() -> FText
						//{
						//	return RoomGenerator->RoomDataAssets[0] ? FText::FromString(*RoomGenerator->RoomDataAssets[0]->GetName()) : LOCTEXT("None", "None");
						//})
					]*/
					
					//SNew(SComboBox<>).HAlign(HAlign_Left).Text(LOCTEXT("Dungeon Preview", "Show Preview"))
					//.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandPreview)
				]
            ]
		]
		+SHorizontalBox::Slot().AutoWidth().Padding(10, 0, 10, 0)
		[
			SNew(SBox)
			.HAlign(HAlign_Left).MaxDesiredWidth(1000)//.WidthOverride(500)
			.VAlign(VAlign_Top).MaxDesiredHeight(400)//.HeightOverride(100)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().AutoHeight()
				.Padding(20,10,0,0)
				[
					SNew(STextBlock).Text(LOCTEXT("RoomCommandsTitle", "Room Generator Commands:"))
				]
				+SVerticalBox::Slot()
				[
					SNew(SSpacer).Size(DGSlateSpacerSize)
				]
				+SVerticalBox::Slot().AutoHeight()
				.Padding(20,0,0,0)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth()
					.Padding(0,0,10,0)
					[
						SNew(STextBlock).Text(LOCTEXT("MakeRoomCommandTitle", "New File:"))
					]
					+SHorizontalBox::Slot().AutoWidth()
					.Padding(0,0,0,0).HAlign(HAlign_Fill)
					[
						SNew(SEditableTextBox).HintText(LOCTEXT("NewFileName", "Insert New File Name Here"))
						.OnTextCommitted_Raw(this, &FDungeonGeneratorModule::RGCommandMakeRoom)
					]					
				]
				+SVerticalBox::Slot()
				[
					SNew(SSpacer).Size(DGSlateSpacerSize)
				]
				+SVerticalBox::Slot().AutoHeight()
				//.Padding(20,0,0,0)
				//[
				//	SNew(SComboBox<TSharedPtr<FString>>).Content()
				//	[
				//		SNew(SListView<TSharedPtr<FString>>)
				//		+SListView::
				//	]
				//	/*.MenuContent()
				//	[
				//		//SNew(SComboRow<TSharedPtr<FString>>)
				//		
				//		SNew(SVerticalBox)
				//		+SVerticalBox::Slot().AutoHeight()
				//		[
				//			SNew(STextBlock).Text(LOCTEXT("1", "Page 1"))
				//		]
				//		+SVerticalBox::Slot().AutoHeight()
				//		[
				//			SNew(STextBlock).Text(LOCTEXT("2", "Page 2"))
				//		]
				//		+SVerticalBox::Slot().AutoHeight()
				//		[
				//			SNew(STextBlock).Text(LOCTEXT("3", "Page 3"))
				//		]
				//	]*/
				//	
				//]
				+SVerticalBox::Slot()
				[
					SNew(SSpacer).Size(DGSlateSpacerSize)
				]
				+SVerticalBox::Slot().AutoHeight()
				.Padding(20,0,0,0)
				[
					SNew(STextBlock).Text_Lambda([this]() -> FText
					{
						return FText::Format(LOCTEXT("NumOfAssetFiles", "Asset Files: {0}"),
															RoomGenerator->GetAssetNum());
					})
				]
				//+SVerticalBox::Slot().AutoHeight().Padding(20,0,0,0)//.HAlign(HAlign_Left)
				//[
				//	SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("SaveCurrentDungeon", "Save Current Dungeon"))
				//	.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandSave)
				//]
			]
		]		
	];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDungeonGeneratorModule, DungeonGenerator)