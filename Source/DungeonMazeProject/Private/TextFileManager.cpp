// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"


FString UTextFileManager::LoadFileToString(FString filepath)
{
	FString directory = FPaths::ProjectContentDir();
	FString result;
	//IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	IFileManager& file = IFileManager::Get();
	
	//if (file.CreateDirectory(*directory))
	if (file.DirectoryExists(*directory))
	{
		FString completefilepath = directory + "/" + filepath;
		FFileHelper::LoadFileToString(result, *completefilepath);
	}

	return result;
}

TArray<FString> UTextFileManager::LoadFileToStringArray(FString filepath)
{
	FString directory = FPaths::ProjectContentDir();
	TArray<FString> result;
	//IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	IFileManager& file = IFileManager::Get();
	
	//if (file.CreateDirectory(*directory))
	if (file.DirectoryExists(*directory))
	{
		FString completefilepath = directory + "/" + filepath;
		FFileHelper::LoadFileToStringArray(result, *completefilepath);
	}

	return result;
}