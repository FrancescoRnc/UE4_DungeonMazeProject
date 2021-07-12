// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCharacter.h"

// Sets default values
ADungeonCharacter::ADungeonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch =
		bUseControllerRotationYaw = 
			bUseControllerRotationRoll = false;
			
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1080.f, 0.f);

}

// Called when the game starts or when spawned
//void ADungeonCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
//void ADungeonCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void ADungeonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Interact);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Attack);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADungeonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADungeonCharacter::MoveRight);

}

void ADungeonCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
}

void ADungeonCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void ADungeonCharacter::MoveForward(float _value)
{	
	if (Controller && _value != 0.0f)
	{
		const FRotator _rotation = Controller->GetControlRotation();
		const FRotator _yawRot(0.f, _rotation.Yaw, 0.f);
		const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::X);

		AddMovementInput(_direction, _value);
	}
}

void ADungeonCharacter::MoveRight(float _value)
{	
	if (Controller && _value != 0.0f)
	{
		const FRotator _rotation = Controller->GetControlRotation();
		const FRotator _yawRot(0.f, _rotation.Yaw, 0.f);
		const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(_direction, _value);
	}
}

