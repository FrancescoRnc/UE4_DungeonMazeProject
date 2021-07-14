// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	if (ObjectData.StaticMesh) StaticMesh->SetStaticMesh(ObjectData.StaticMesh);

	BoxInteractionCollider = CreateDefaultSubobject<UBoxComponent>("Interaction Collider");
	BoxInteractionCollider->SetupAttachment(RootComponent);
	BoxInteractionCollider->SetBoxExtent(ObjectData.ColliderSize);
	BoxInteractionCollider->SetCollisionProfileName("Interaction");

}

// Called when the game starts or when spawned
//void AInteractableObject::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
//void AInteractableObject::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AInteractableObject::Interact_Implementation()
{
	
}

