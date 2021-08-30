// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "DungeonCamera.generated.h"

UCLASS()
class DUNGEONMAZEPROJECT_API ADungeonCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonCamera();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TargetPoint;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AttachToActor(AActor* _other);

	UFUNCTION(BlueprintCallable)
	void DetachFromActor(AActor* _other);
	

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
