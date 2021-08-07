// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoor.h"

#include "Engine/LevelStreaming.h"

// Sets default values
ADungeonDoor::ADungeonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	BoxInteractionColision = CreateDefaultSubobject<UBoxComponent>("Box Interaction");
	FAttachmentTransformRules rules{EAttachmentRule::KeepRelative, false};
	BoxInteractionColision->AttachToComponent(MeshComponent, rules);
	BoxInteractionColision->SetCollisionProfileName(TEXT("Interaction"));
	BoxInteractionColision->SetGenerateOverlapEvents(true);

	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void ADungeonDoor::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void ADungeonDoor::Interact_Implementation()
{
	//UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	UE_LOG(LogTemp, Warning, TEXT("Go To %s"), *Info.RoomDestinationName.ToString());
}

void ADungeonDoor::Initialize(const FDoorInfo& _info)
{
	Info = _info;
	MeshComponent->SetStaticMesh(Info.DoorAsset->StaticMesh);
    BoxInteractionColision->SetBoxExtent(Info.DoorAsset->ColliderSize);
}
