// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interactable.h"
#include "Components/BoxComponent.h"

#include "InteractableObject.generated.h"



USTRUCT(BlueprintType)
struct DUNGEONMAZEPROJECT_API FInteractableData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ColliderSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInteractionDelegate Delegate;
};


UCLASS()
class DUNGEONMAZEPROJECT_API AInteractableObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
	UBoxComponent* BoxInteractionCollider;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Object Data")
	FInteractableData ObjectData;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;

};
