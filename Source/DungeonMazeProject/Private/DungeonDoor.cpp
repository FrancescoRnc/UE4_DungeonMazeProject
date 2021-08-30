// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonDoor.h"

#include "DungeonGameInstance.h"
#include "GameFramework/Character.h"


// Sets default values
ADungeonDoor::ADungeonDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Door Mesh");
	BoxInteractionCollision = CreateDefaultSubobject<UBoxComponent>("Box Interaction");
	
	BoxInteractionCollision->SetCollisionProfileName(TEXT("Interaction"));
	BoxInteractionCollision->SetGenerateOverlapEvents(true);

	SetRootComponent(MeshComponent);
	FAttachmentTransformRules rules{EAttachmentRule::KeepRelative, false};
    BoxInteractionCollision->AttachToComponent(MeshComponent, rules);


	//InteractDelegate.BindUFunction();
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

	GoToRoom(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ADungeonDoor::Initialize(const FDoorInfo& _info)
{
	Info = _info;
	MeshComponent->SetStaticMesh(Info.DoorAsset->StaticMesh);
    BoxInteractionCollision->SetBoxExtent(Info.DoorAsset->ColliderSize);
	BoxInteractionCollision->AddLocalOffset({-(Info.DoorAsset->ColliderSize.X * .5f), 0, Info.DoorAsset->ColliderSize.Z});
}

void ADungeonDoor::GoToRoom(ACharacter* character)
{
	FVector newPos = LinkedDoor->GetActorLocation() - (LinkedDoor->GetActorForwardVector() * 100) + FVector(0, 0, 100);
	//FRotator newRot = LinkedDoor->GetActorForwardVector().Rotation().GetInverse();
	//character->SetActorLocationAndRotation(newPos, newRot);
	character->SetActorLocation(newPos);
}
