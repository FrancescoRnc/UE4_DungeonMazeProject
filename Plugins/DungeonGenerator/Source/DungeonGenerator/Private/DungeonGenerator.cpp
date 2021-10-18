// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGenerator.h"

#define LOCTEXT_NAMESPACE "FDungeonGeneratorModule"

void FDungeonGeneratorModule::StartupModule()
{

}

void FDungeonGeneratorModule::ShutdownModule()
{
	
}

bool FDungeonGeneratorModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	const FString CmdAsString = Cmd;

	if (CmdAsString == "generate")
	{
		UE_LOG(LogTemp, Display, TEXT("A NEW MAZE HAS BEEN GENERATED!"));
		return true;
	}
	else if (CmdAsString == "save")
	{
		UE_LOG(LogTemp, Warning, TEXT("CURRENT MAZE HAS BEEN SAVED!"));
		return true;
	}
	else if (CmdAsString == "reset")
	{
		UE_LOG(LogTemp, Warning, TEXT("CURRENT MAZE HAS BEEN RESET!"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Command Not Found..."));
		return false;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDungeonGeneratorModule, DungeonGenerator)