// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputHandle.h"

#include "DungeonCharacter.generated.h"

/**
 * Dungeon Character: This is the Character possessed by the Dungeon Player Controller.
 * 
 * @see ADungeonPlayerController
 */
UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonCharacter : public ACharacter
{

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonCharacter();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
	UCapsuleComponent* InteractionCapsule;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	APlayerCameraManager* CameraManager;

	InputHandle PlayerInputHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Interact();
	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void MoveForward(const float _value);
	UFUNCTION(BlueprintCallable)
	void MoveRight(const float _value);


	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// NOT USED FOR NOW
	UFUNCTION(BlueprintCallable)
	void ActivateCharacter();
	UFUNCTION(BlueprintCallable)
	void DeactivateCharacter();
};
