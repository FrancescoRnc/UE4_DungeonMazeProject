// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGenerator.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DungeonRoom.h"
#include "DungeonSchemeMaker.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "SRoomSelectorCombo.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/LayerManager/SLayerManager.h"
#include "Widgets/Layout/SSpacer.h"


#define LOCTEXT_NAMESPACE "FDungeonGeneratorModule"


// FRoomGenerator
FRoomGenerator::FRoomGenerator()
{

}

FReply FRoomGenerator::DGCommandRoomPreview()
{
	USelection* Selection = GEditor->GetSelectedActors();
	UObject* SelectedObject = Selection->GetTop(ADungeonRoom::StaticClass());
	ADungeonRoom* SelectedRoom;
	
	if (SelectedObject)
	{
		SelectedRoom = Cast<ADungeonRoom>(SelectedObject);
	}
	else
	{
		UWorld * World = GEditor->GetEditorWorldContext().World();
		SelectedRoom = World->SpawnActor<ADungeonRoom>(ADungeonRoom::StaticClass(), FTransform::Identity);
	}

	const URoomPresetPtr SelectedPreset = FDungeonUtils::Get()->GetSelectedPreset();
	if (SelectedPreset)
	{
		FRoomInfo Info;
		//Info.Preset = MakeShared<URoomPresetPtr>(SelectedPreset);
		Info.PresetID = SelectedPreset->PresetID;
		//Info.PresetName = SelectedPreset->GetFName();
		Info.PresetPath = *SelectedPreset->GetPathName();
		SelectedRoom->InsertData(Info);
		SelectedRoom->Generate(SelectedPreset);
		
		UE_LOG(LogTemp, Display, TEXT("HERE'S A PREVIEW OF THE SELECTED ROOM!"));
	}	
	
	return FReply::Handled();
}

ADungeonRoom* FRoomGenerator::Generate(const FRoomInfo& Info)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();

	FActorSpawnParameters Parameters;
	Parameters.Name = Info.RoomName;
	ADungeonRoom* Room = World->SpawnActor<ADungeonRoom>
		(ADungeonRoom::StaticClass(), FTransform::Identity);	
	
	Room->InsertData(Info);
	Room->Locate();
	Room->Show();

	return Room;
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
	FDungeonGridMaker DungeonGridMaker(10);
	FGrid Grid = DungeonGridMaker.GetGrid();
	const TArray<int32> Scheme = Grid.GetScheme();
	
	OutDungeonInfo = {};
	OutDungeonInfo.Grid = Grid;
	OutDungeonInfo.GridSize = Grid.Size;	
	OutDungeonInfo.GridScheme = Scheme;

	// PROSEGUIRE CON LA SERIALIZZAZIONE, GENERAZIONE DELLE DOORS E LINKING TRA DOORS 

	UE_LOG(LogTemp, Warning, TEXT("+--------------------------------+"));
	UE_LOG(LogTemp, Warning, TEXT("Scheme"));
	for (int32 Y = Grid.Size.Y - 1; Y >= 0; Y--)
	{
		FString Row("");
		for (int32 X = Grid.Size.X - 1; X >= 0; X--)
		{
			const int32 Index = X + (Y * Grid.Size.X);
			Row.Append(Scheme[Index] == 0 ?
				TEXT("[--]") : FString::Printf(TEXT("[%02d]"), Scheme[Index]));
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Row);
	}
	
	UE_LOG(LogTemp, Display, TEXT("Building Dungeon..."));
}

void FDungeonGenerator::BuildRooms(TArray<FRoomInfo>& OutRoomsInfo/*, const TArray<URoomPresetPtr>& Presets*/)
{
	UE_LOG(LogTemp, Display, TEXT("Building Rooms..."));
	
	for (int32 Index = 0; Index < OutDungeonInfo.Grid.PathLength; Index++)
	{	
		FRoomInfo Room;
		const int32 RandomPresetsMax = FDungeonUtils::Get()->GetPresetsCount() - 1;
		const int32 RandomPresetIndex = FMath::RandRange(0, RandomPresetsMax);
		const URoomPresetPtr RandomPreset = FDungeonUtils::Get()->GetRoomPresets()[RandomPresetIndex];
		const FIntVector RoomCoords =
		{
		OutDungeonInfo.Grid.PathTrack[Index].X,
		OutDungeonInfo.Grid.PathTrack[Index].Y,
		0
		};
		
		//Room.RoomName = *FString::Printf(TEXT("%s_0"), *RandomPreset->GetName());
		Room.RoomName = *FString::Printf(TEXT("PathOrder%02d"), (Index + 1));
		Room.CoordinateInGrid = RoomCoords; //OutDungeonInfo.Grid.PathTrack[Index];
		Room.IndexInGrid = Room.CoordinateInGrid.X +
			(Room.CoordinateInGrid.Y * OutDungeonInfo.GridSize.X);
		Room.PresetPath = *RandomPreset->GetPathName();
		
		OutDungeonInfo.RoomsInfo.Add(Room);
	}
	OutRoomsInfo = OutDungeonInfo.RoomsInfo;
	
	UE_LOG(LogTemp, Display, TEXT("Rooms Built!"));
}

void FDungeonGenerator::BuildDoors(TArray<FDoorInfo>& OutDoorsInfo)
{
	UE_LOG(LogTemp, Display, TEXT("Building Doors..."));

	TArray<FRoomInfo>& RefRoomsInfo = OutDungeonInfo.RoomsInfo;
	TArray<FDoorInfo> DoorsInfo;
	
	/*for (int32 Index = 0; Index < OutDungeonInfo.Grid.PathLength; Index++)
	{
		FRoomInfo& Room = RefRoomsInfo[Index];
		const FIntVector Coord = OutDungeonInfo.Grid.PathTrack[Index];
		const int32 RoomPattern = OutDungeonInfo.Grid.GetCell(Coord.X, Coord.Y).CellPattern;
		
		TArray<int32> Patterns;
		for (int32 PatternID = 1; PatternID < 9; PatternID = PatternID << 1)
		{			
			if ((RoomPattern & PatternID) == PatternID)
			{
				Patterns.Add(PatternID);
			}			
		}
		const int32 DoorsCount = Patterns.Num();

		
		for (int32 DoorIndex = 0; DoorIndex < DoorsCount; DoorIndex++)
		{			
			FDoorInfo Door;
			Door.SourceRoomName = Room.RoomName;
			//Door.NextRoomName = Room.RoomName;
			Door.SourceRoomIndex = Index;
			Door.Direction = static_cast<EWorldDirection>(Patterns[DoorIndex]);			

			DoorsInfo.Add(Door);
			Room.DoorsInfo.Add(Door);
		}
	}
	DoorsInfo[0].NextRoomIndex = 1;
	for (int32 Index = 1; Index < DoorsInfo.Num(); Index++)
	{
		

		
	}*/

	for (int32 RoomIndex = 1; RoomIndex < RefRoomsInfo.Num(); RoomIndex++)
	{
		const int32 PrevRoomIndex = RoomIndex - 1;
		//UE_LOG(LogTemp, Display, TEXT("Index: %d | PrevIndex: %d"), RoomIndex, PrevRoomIndex);
		FRoomInfo& PrevRoom = RefRoomsInfo[PrevRoomIndex];
		FRoomInfo& CurrRoom = RefRoomsInfo[RoomIndex];
		FDoorInfo PrevDoor, CurrDoor;

		const FIntVector DeltaCoords =
			CurrRoom.CoordinateInGrid - PrevRoom.CoordinateInGrid;
		const int32 PatternIndex = Directions.IndexOfByKey(DeltaCoords);

		UE_LOG(LogTemp, Display, TEXT("Index: %d | Delta: %s"), PatternIndex, *DeltaCoords.ToString());

		PrevDoor.SourceRoomIndex = PrevRoomIndex;
		PrevDoor.NextRoomIndex = RoomIndex;
		const int32 CurrDirection =
			FDungeonGridMaker::GetPatternFromIndex(PatternIndex);
		PrevDoor.Direction = static_cast<EWorldDirection>(CurrDirection);
		
		CurrDoor.SourceRoomIndex = RoomIndex;
		CurrDoor.NextRoomIndex = PrevRoomIndex;
		const int32 PrevDirection =
			FDungeonGridMaker::GetPatternFromIndex(
				FDungeonGridMaker::GetOppositeIndex(PatternIndex));;
		CurrDoor.Direction = static_cast<EWorldDirection>(PrevDirection);

		PrevRoom.DoorsInfo.Add(PrevDoor);
		CurrRoom.DoorsInfo.Add(CurrDoor);
	}
	
	UE_LOG(LogTemp, Display, TEXT("Doors Built!"));
}


// FDungeonGeneratorModule
void FDungeonGeneratorModule::StartupModule()
{	
	DungeonGenerator	= MakeShared<FDungeonGenerator>();
	RoomGenerator		= MakeShared<FRoomGenerator>();
	DungeonUtils		= MakeShared<FDungeonUtils>();
	//DungeonUtils->RescanAssetReferences();

	FAssetRegistryModule::GetRegistry().OnFilesLoaded()
		.AddRaw(DungeonUtils.Get(), &FDungeonUtils::GetPresetsOnLoad);
	FAssetRegistryModule::GetRegistry().OnInMemoryAssetDeleted()
		.AddLambda([this](UObject* Object)
    {    	
    	if (const URoomPresetPtr Preset = Cast<URoomPreset>(Object))
    	{
    		DungeonUtils.Get()->DeleteAssetReference(Preset);
    		UE_LOG(LogTemp, Warning, TEXT("In Memory Asset Deleted: %s"), *Object->GetName());
    	}    	
    });
	
	/*// DEBUG AssetRegistry Events
	FAssetRegistryModule::GetRegistry().OnAssetAdded().AddLambda([](const FAssetData& Data)
		{UE_LOG(LogTemp, Warning, TEXT("On Asset Added: %s"), *Data.AssetName.ToString());});
	FAssetRegistryModule::GetRegistry().OnAssetRemoved().AddLambda([](const FAssetData& Data)
		{UE_LOG(LogTemp, Warning, TEXT("On Asset Removed: %s"), *Data.AssetName.ToString());});
	FAssetRegistryModule::GetRegistry().OnAssetUpdated().AddLambda([](const FAssetData& Data)
		{UE_LOG(LogTemp, Warning, TEXT("On Asset Updated: %s"), *Data.AssetName.ToString());});
	FAssetRegistryModule::GetRegistry().OnAssetRenamed().AddLambda([](const FAssetData& Data,  const FString& NewName)
		{UE_LOG(LogTemp, Warning, TEXT("On Asset Renamed: %s -> %s"), *Data.AssetName.ToString(), *NewName);});
	FAssetRegistryModule::GetRegistry().OnFilesLoaded().AddLambda([]()
		{UE_LOG(LogTemp, Warning, TEXT("On Files Loaded") );});
	FAssetRegistryModule::GetRegistry().OnPathAdded().AddLambda([](const FString& Path)
		{UE_LOG(LogTemp, Warning, TEXT("On Path Added: %s"), *Path);});
	FAssetRegistryModule::GetRegistry().OnPathRemoved().AddLambda([](const FString& Path)
		{UE_LOG(LogTemp, Warning, TEXT("On Path Removed: %s"), *Path);});
	//FAssetRegistryModule::GetRegistry().OnFileLoadProgressUpdated().AddLambda([](const FFileLoadProgressUpdateData&)
	//	{});
	FAssetRegistryModule::GetRegistry().OnInMemoryAssetCreated().AddLambda([](UObject* Object)
		{UE_LOG(LogTemp, Warning, TEXT("On In Memory Asset Created: %s"), *Object->GetName());});
	FAssetRegistryModule::GetRegistry().OnInMemoryAssetDeleted().AddLambda([](UObject* Object)
		{UE_LOG(LogTemp, Warning, TEXT("On In Memory Asset Deleted: %s"), *Object->GetName());});
	// END DEBUG*/
	
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
	
	if (CmdAsString == "queryfiles")
	{
		UE_LOG(LogTemp, Display, TEXT("Number of Asset Files: %d | Path: %s"),
			DungeonUtils->GetPresetsCount(), *DungeonUtils->PrefabricsPath.ToString());
		for (const URoomPresetPtr Data : DungeonUtils->GetRoomPresets())
		{
			UE_LOG(LogTemp, Display, TEXT("Asset File: %s"), *Data->GetName());
		}
	 	UE_LOG(LogTemp, Display, TEXT("Scan status: %d"), DungeonUtils->RescanAssetReferences());
		
		return true;
	}
	else if (CmdAsString == "querydungeon")
	{
		UE_LOG(LogTemp, Display, TEXT("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"));
		UE_LOG(LogTemp, Display, TEXT("Querying Dungeon Information..."));
		
		UE_LOG(LogTemp, Display, TEXT("Dungeon Grid Size:							%s"),
			*CurrentDungeonInfo.GridSize.ToString());
		
		const int32 RoomsCount = CurrentDungeonInfo.RoomsInfo.Num();
		UE_LOG(LogTemp, Display, TEXT("Dungeon Rooms Number:						%d"),
			RoomsCount);
		
		for (int32 Index = 0; Index < RoomsCount; Index++)
		{
			const FRoomInfo CurrentRoom = CurrentDungeonInfo.RoomsInfo[Index];
			
			//const URoomPresetPtr GetPreset = *CurrentRoom.Preset.Get();
			//const URoomPresetPtr GetPreset = DungeonUtils->GetRoomPreset(CurrentRoom.PresetID);
			//const URoomPresetPtr GetPreset = DungeonUtils->GetRoomPresetByName(CurrentRoom.PresetName);
			const URoomPresetPtr GetPreset = DungeonUtils->GetRoomPresetByPath(CurrentRoom.PresetPath);
			UE_LOG(LogTemp, Display, TEXT("-|- Room of index %d Preset Used:			%s"),
				Index, *GetPreset->GetName());

			UE_LOG(LogTemp, Display, TEXT("-|- Room of index %d Name:					%s"),
				Index, *CurrentRoom.RoomName.ToString());

			UE_LOG(LogTemp, Display, TEXT("-|- Room of index %d Coords in Grid:			%s"),
				Index, *CurrentRoom.CoordinateInGrid.ToString());

			UE_LOG(LogTemp, Display, TEXT("-|- Room of index %d Index in Grid:			%d"),
				Index, CurrentRoom.IndexInGrid);

			const int32 DoorsCount = CurrentRoom.DoorsInfo.Num();
			UE_LOG(LogTemp, Display, TEXT("-|- Room of index %d Number of Doors:		%d"),
				Index, DoorsCount);

			for (int32 JIndex = 0; JIndex < DoorsCount; JIndex++)
			{
				const FDoorInfo CurrentDoor = CurrentRoom.DoorsInfo[JIndex];

				FString DirectionName;
				switch (CurrentDoor.Direction)
				{
					case EWorldDirection::NORTH:
						DirectionName = TEXT("NORTH");
						break;
					case EWorldDirection::EAST:
                    	DirectionName = TEXT("EAST");
                    	break;
                    case EWorldDirection::SOUTH:
                    	DirectionName = TEXT("SOUTH");
                    	break;
					case EWorldDirection::WEST:
						DirectionName = TEXT("WEST");
						break;
					
					default:
						DirectionName = TEXT("NONE");
						break;
				}
				
				UE_LOG(LogTemp, Display, TEXT("-|- -|- Door at %s Destination Room:		%s"),
				*DirectionName, *CurrentDoor.NextRoomName.ToString());
			}
		}
		UE_LOG(LogTemp, Display, TEXT("/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-"));
		
		return true;
	}

	return false;
}


// Commands
FReply FDungeonGeneratorModule::DGCommandGenerate()
{
	//TArray<FRoomInfo> OutRoomsInfo;
	//TArray<FDoorInfo> OutDoorsInfo;
		
	DungeonGenerator->BuildDungeon();
	DungeonGenerator->BuildRooms(RoomsInfo);
	DungeonGenerator->BuildDoors(DoorsInfo);
	CurrentDungeonInfo = DungeonGenerator->GetDungeonInfo();	

	UE_LOG(LogTemp, Display, TEXT("A NEW DUNGEON HAS BEEN GENERATED!"));	
	
	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandSave()
{
	// Save Operations
	
	UE_LOG(LogTemp, Display, TEXT("CURRENT DUNGEON HAS BEEN SAVED!"));

	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandReset()
{
	// Reset Operations

	UE_LOG(LogTemp, Display, TEXT("CURRENT DUNGEON RESET TO PREVIOUS STATE!"));
	
	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandPreview()
{
	// Preview Operations

	// Spawn Rooms with Doors
	const int32 RoomsCount = RoomsInfo.Num();
	for (int32 Index = 0; Index < RoomsCount; Index++)
	{
		const FRoomInfo CurrentRoomInfo = RoomsInfo[Index];
		/*const URoomPresetPtr Preset = DungeonUtils->GetRoomPresetByName(CurrentRoomInfo.PresetName);
		const URoomPresetPtr Preset = DungeonUtils->GetRoomPresetByPath(CurrentRoomInfo.PresetPath);
		ADungeonRoom* RoomActor = World->SpawnActor<ADungeonRoom>(
			ADungeonRoom::StaticClass(), FTransform::Identity);
		RoomActor->InsertData(CurrentRoomInfo);
		RoomActor->Locate();
		RoomActor->Generate(Preset);*/

		ADungeonRoom* Room = RoomGenerator->Generate(CurrentRoomInfo);

		const int32 DoorsCount = CurrentRoomInfo.DoorsInfo.Num();
		for (int32 JIndex = 0; JIndex < DoorsCount; JIndex++)
		{
			// Spawn/Show Doors
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT("HERE'S A PREVIEW OF THE MAP!"));
	
	return FReply::Handled();
}

FReply FDungeonGeneratorModule::DGCommandRoomPreview()
{
	USelection* Selection = GEditor->GetSelectedActors();
	UObject* SelectedObject = Selection->GetTop(ADungeonRoom::StaticClass());
	ADungeonRoom* SelectedRoom;
	
	if (SelectedObject)
	{
		SelectedRoom = Cast<ADungeonRoom>(SelectedObject);
	}
	else
	{
		UWorld * World = GEditor->GetEditorWorldContext().World();
		SelectedRoom = World->SpawnActor<ADungeonRoom>(ADungeonRoom::StaticClass(), FTransform::Identity);
	}

	const URoomPresetPtr SelectedPreset = FDungeonUtils::Get()->GetSelectedPreset();
	if (SelectedPreset)
	{
		FRoomInfo Info;
		//Info.Preset = MakeShared<URoomPresetPtr>(SelectedPreset);
		Info.PresetID = SelectedPreset->PresetID;
		//Info.PresetName = SelectedPreset->GetFName();
		Info.PresetPath = *SelectedPreset->GetPathName();
		SelectedRoom->InsertData(Info);
		//SelectedRoom->Generate(SelectedPreset);
		SelectedRoom->Show();
		
		UE_LOG(LogTemp, Display, TEXT("HERE'S A PREVIEW OF THE SELECTED ROOM!"));
	}	
	
	return FReply::Handled();
}

void FDungeonGeneratorModule::RGCommandMakeRoom(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		//RoomGenerator->RGCommandMakeRoom(InText.ToString());
		DungeonUtils->RGCommandMakeRoom(InText.ToString());
	}
}


TSharedRef<SDockTab> FDungeonGeneratorModule::SpawnNomadTab(const FSpawnTabArgs& TabSpawnArgs)
{		
	FToolBarBuilder ToolbarBuilder(MakeShared<FUICommandList>(), FMultiBoxCustomization::None);

	// Dungeon Generation Features
	const FText DShowPreviewText = LOCTEXT("ShowPreview", "Show Preview");
	ToolbarBuilder.AddToolBarButton(FUIAction(FExecuteAction::CreateLambda([this]()
	{
		DGCommandPreview();
	})), NAME_None, DShowPreviewText);
	
	ToolbarBuilder.AddSeparator();
	
	// Room Generation Features
	const FText RShowPreviewText = LOCTEXT("ShowRoomPreview", "Room Preview");
	ToolbarBuilder.AddToolBarButton(FUIAction(FExecuteAction::CreateLambda([this]()
	{
		//RoomGenerator->DGCommandRoomPreview();
		DGCommandRoomPreview();
	})), NAME_None, RShowPreviewText);

	ToolbarBuilder.AddSeparator();
	
	const TSharedRef<SWidget> Toolbar = ToolbarBuilder.MakeWidget();
	

	const TSharedRef<SRoomSelectorCombo<URoomPresetPtr>> RoomSelectorCombo =
		SNew(SRoomSelectorCombo<URoomPresetPtr>).Source(&DungeonUtils->RoomPresets);
	RoomSelectorCombo->OnOptionSelected.BindLambda([this](const URoomPresetPtr Preset)
	{
		DungeonUtils->SelectedPreset = Preset;
	});

	
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot().AutoHeight()
		[
			Toolbar
		]
		+SVerticalBox::Slot()
		[		
			SNew(SHorizontalBox)
			// Dungeon Generation Features
			+SHorizontalBox::Slot().AutoWidth().Padding(HPadding, 0, HPadding, 0)
			[
				SNew(SBox)
        	    .HAlign(HAlign_Left)
        	    .VAlign(VAlign_Top)
        	    [
        	    	SNew(SVerticalBox)
        	    	+SVerticalBox::Slot().AutoHeight().Padding(0,VPadding,0,0)//.HAlign(HAlign_Left)
        	    	[
        	    		SNew(STextBlock).Text(LOCTEXT("DungeonCommandsTitle", "Dungeon Generator Commands:"))
        	    	]
        	    	+SVerticalBox::Slot()
        	    	[
        	    		SNew(SSpacer).Size(DGSpacerSize)
        	    	]
        	    	+SVerticalBox::Slot().AutoHeight().Padding(0,0,0,0)//.HAlign(HAlign_Left)
        	    	[
        	    		SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("GenerateNewDungeon", "Generate New Dungeon"))
        	    		.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandGenerate)
        	    	]
        	    	+SVerticalBox::Slot()
        	    	[
        	    		SNew(SSpacer).Size(DGSpacerSize)
        	    	]
        	    	+SVerticalBox::Slot().AutoHeight().Padding(0,0,0,0)//.HAlign(HAlign_Left)
        	    	[
        	    		SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("SaveCurrentDungeon", "Save Current Dungeon"))
        	    		.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandSave)
        	    	]
        	    	+SVerticalBox::Slot()
					[
						SNew(SSpacer).Size(DGSpacerSize)
					]
					+SVerticalBox::Slot().AutoHeight().Padding(0,0,0,0)//.HAlign(HAlign_Left)
					[
						SNew(SButton).HAlign(HAlign_Left).Text(LOCTEXT("ResetCurrentDungeon", "Reset Current Dungeon"))
						.OnClicked_Raw(this, &FDungeonGeneratorModule::DGCommandReset)
					]
        	    ]
			]
			// Room Generation Features
			+SHorizontalBox::Slot().AutoWidth().Padding(HPadding, 0, HPadding, 0)
			[
				SNew(SBox)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot().AutoHeight()//.HAlign(HAlign_Left)
					.Padding(0,VPadding,0,0)
					[
						SNew(STextBlock).Text(LOCTEXT("RoomCommandsTitle", "Room Generator Commands:"))
					]
					+SVerticalBox::Slot()
					[
						SNew(SSpacer).Size(DGSpacerSize)
					]
					+SVerticalBox::Slot().AutoHeight()//.HAlign(HAlign_Left)
					.Padding(0,0,0,0)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot().AutoWidth()
						.Padding(0,0,HPadding,0)
						[
							SNew(STextBlock).Text(LOCTEXT("MakeRoomCommandTitle", "New File:"))
						]
						+SHorizontalBox::Slot().HAlign(HAlign_Fill)
						.Padding(0,0,0,0)
						[
							SNew(SEditableTextBox).HintText(LOCTEXT("NewFileName", "New File Name"))
							.OnTextCommitted_Raw(this, &FDungeonGeneratorModule::RGCommandMakeRoom)
						]					
					]
					+SVerticalBox::Slot()
					[
						SNew(SSpacer).Size(DGSpacerSize)
					]
					+SVerticalBox::Slot().AutoHeight()//.HAlign(HAlign_Left)
					.Padding(0,0,0,0)
					[
						SNew(STextBlock).Text_Lambda([this]() -> FText
						{
							return FText::Format(LOCTEXT("NumOfAssetFiles", "Asset Files: {0}"),
																DungeonUtils->GetPresetsCount());
						})
					]
					+SVerticalBox::Slot()
					[
						SNew(SSpacer).Size(DGSpacerSize)
					]
					+SVerticalBox::Slot().AutoHeight()//.HAlign(HAlign_Left)
					.Padding(0,0,0,0)
					[
						RoomSelectorCombo
						/*SNew(SComboBox<URoomPresetPtr>)
						.OptionsSource(&DungeonUtils->RoomPresets)
						.OnSelectionChanged_Lambda([this](const URoomPresetPtr Preset)
						{
							DungeonUtils->SelectedPreset = Preset;
						})
						.OnGenerateWidget_Lambda([this](URoomPresetPtr InOption)
						{
							return SNew(STextBlock).Text(FText::FromString(*InOption->GetName()));
						})
						.InitiallySelectedItem(DungeonUtils->RoomPresets[0])
						[
							SNew(STextBlock)//.Text(LOCTEXT("None", "None"))
							.Text_Lambda([this]() -> FText
							{
								return DungeonUtils->RoomPresets[0] ?
									FText::FromString(*DungeonUtils->RoomPresets[0]->GetName()) :
									LOCTEXT("None", "None");
							})
						]*/
					]
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDungeonGeneratorModule, DungeonGenerator)