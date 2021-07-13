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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	InteractionCollider = CreateDefaultSubobject<USphereComponent>("Interaction Collider");
	InteractionCollider->InitSphereRadius(100.f);
	InteractionCollider->SetCollisionProfileName(TEXT("Interaction"));
	InteractionCollider->SetupAttachment(RootComponent);
	InteractionCollider->SetGenerateOverlapEvents(true);
	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionBeginOverlap);
    InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionEndOverlap);


	
}

// Called when the game starts or when spawned
void ADungeonCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

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

	PlayerInputHandle = (PlayerInputComponent);
	PlayerInputHandle.RegisterNewAction("Interaction", EInputEvent::IE_Pressed);
	PlayerInputHandle.RegisterNewAction("Attack", EInputEvent::IE_Pressed);
	PlayerInputHandle.GetActionData(0).Delegate.BindUObject(this, &ADungeonCharacter::Interact);
	PlayerInputHandle.GetActionData(1).Delegate.BindUObject(this, &ADungeonCharacter::Attack);
	PlayerInputHandle.SetDelegateToAction(0);
	PlayerInputHandle.SetDelegateToAction(1);
	
	//PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Interact);
	//PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Attack);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ADungeonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADungeonCharacter::MoveRight);
	
	//PlayerInputHandle.AddData({0, "Interaction"});
	//FInputActionHandlerSignature signature; signature.BindUObject(this, &ADungeonCharacter::Interact);
	//PlayerInputHandle.SetDelegateToData(0, signature);
	//PlayerInputHandle.AddData({1, "Attack"});
}



void ADungeonCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
}
void ADungeonCharacter::NewInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction but with a new function!!!"));
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

void ADungeonCharacter::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Something has entered Collision!"));
	PlayerInputHandle.GetActionData(0).Delegate.BindUObject(this, &ADungeonCharacter::NewInteract);
	PlayerInputHandle.SetDelegateToAction(0);
}

void ADungeonCharacter::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Something has exited Collision!"));
	PlayerInputHandle.GetActionData(0).Delegate.BindUObject(this, &ADungeonCharacter::Interact);
	PlayerInputHandle.SetDelegateToAction(0);
}

void ADungeonCharacter::SetInteractionDelegate(FInteractionDelegate _delegate)
{
	FInputActionHandlerSignature newSignature;
	newSignature.BindUFunction(_delegate.GetUObject(), _delegate.GetFunctionName());
	InputComponent->GetActionBinding(0).ActionDelegate = newSignature;
}

