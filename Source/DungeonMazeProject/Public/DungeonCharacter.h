// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "InputHandle.h"

#include "DungeonCharacter.generated.h"


//DECLARE_DYNAMIC_DELEGATE(FInteractionDelegate);


UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonCharacter : public ACharacter
{

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonCharacter();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USphereComponent* InteractionCollider;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	APlayerCameraManager* CameraManager;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	InputHandle PlayerInputHandle;
	


	UFUNCTION(BlueprintCallable)
	void Interact();
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void MoveForward(float _value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(float _value);


	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//UFUNCTION()
	//void SetInteractionDelegate(FInteractionDelegate delegate);

};
