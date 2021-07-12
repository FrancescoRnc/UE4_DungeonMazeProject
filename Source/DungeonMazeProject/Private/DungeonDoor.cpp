// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoor.h"

// Sets default values
ADungeonDoor::ADungeonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//UWorld* world = GetWorld();
	//CurrentLevelName = FName(world->GetName());
}

void ADungeonDoor::GetInteraction_Implementation(AActor* other)
{
	FLatentActionInfo info;
	//UGameplayStatics::LoadStreamLevel(this, NextLevelName, true, true, info);
	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
}

// Called when the game starts or when spawned
//void ADungeonDoor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
//void ADungeonDoor::Tick(float DeltaTime)
//{
//
//	Super::Tick(DeltaTime);
//
//}

