// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCrate.h"

// Sets default values
ADungeonCrate::ADungeonCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADungeonCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void ADungeonCrate::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


void ADungeonCrate::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("YOU OPENED A TREASURE CHEST !!!"));
}



