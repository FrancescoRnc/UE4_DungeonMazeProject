// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

#include "TextFileManager.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMAZEPROJECT_API UTextFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable, Category="File I/O")
	static FString LoadFileToString(FString filepath);
	
	UFUNCTION(BlueprintCallable, Category="File I/O")
	static TArray<FString> LoadFileToStringArray(FString filepath);
};
