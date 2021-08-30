// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	BoxInteractionCollider = CreateDefaultSubobject<UBoxComponent>("Interaction Collider");

	if (RootComponent)
	{
		BoxInteractionCollider->SetupAttachment(RootComponent);
      	StaticMesh->SetupAttachment(RootComponent);
	}
	
	
	if (!ObjectInfo) return;	
	if (ObjectInfo->StaticMesh) StaticMesh->SetStaticMesh(ObjectInfo->StaticMesh);        
        
    
    BoxInteractionCollider->SetBoxExtent(ObjectInfo->ColliderSize);
    BoxInteractionCollider->SetCollisionProfileName("Interaction");
}

void AInteractableObject::Interact_Implementation()
{
	
}


