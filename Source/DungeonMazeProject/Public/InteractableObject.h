// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataAsset.h"
#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "InteractableObject.generated.h"



UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API UInteractableData : public UDataAsset, public IInteractable
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ColliderSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInteractionDelegate Delegate;

	UFUNCTION(BlueprintCallable)
	void doSomething()
	{
		Delegate.ExecuteIfBound();
	}

	
};

UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API UDoorInteractableData : public UInteractableData
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName NextRoomName;

	virtual void Interact_Implementation() override
	{
		UGameplayStatics::OpenLevel(GetWorld(), NextRoomName);
	}
		
};

UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API UCrateInteractableData : public UInteractableData
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Content;

	virtual void Interact_Implementation() override
	{
		UE_LOG(LogTemp, Warning, TEXT("YOU OPENED A TREASURE CHEST !!!"));
	}
};



UCLASS(BlueprintType)
class DUNGEONMAZEPROJECT_API AInteractableObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category="Components")
	UBoxComponent* BoxInteractionCollider;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Object Data Info")
	UInteractableData* ObjectInfo;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;


};
