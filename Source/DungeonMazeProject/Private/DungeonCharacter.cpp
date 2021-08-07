// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCharacter.h"

#include "Interactable.h"
#include "InteractableObject.h"

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
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	

	//InteractionCollider = CreateDefaultSubobject<USphereComponent>("Interaction Collider");
	//InteractionCollider->InitSphereRadius(150.f);
	//InteractionCollider->SetCollisionProfileName(TEXT("Interaction"));
	//InteractionCollider->SetupAttachment(RootComponent);
	//InteractionCollider->SetGenerateOverlapEvents(true);
	//InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionBeginOverlap);
    //InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionEndOverlap);

	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>("Interaction Capsule Collider");
	InteractionCapsule->InitCapsuleSize(35, 100);
	InteractionCapsule->AddRelativeLocation({75, 0, 0});
	InteractionCapsule->SetCollisionProfileName(TEXT("Interaction"));
	InteractionCapsule->SetupAttachment(RootComponent);
	InteractionCapsule->SetGenerateOverlapEvents(true);
	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionBeginOverlap);
	InteractionCapsule->OnComponentEndOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionEndOverlap);
	
}

// Called when the game starts or when spawned
void ADungeonCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation({0,0,100});
	
	if (GEngine && !CameraManager) CameraManager = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
	FLinearColor black{0.0f, 0.0f, 0.0f, 0.0f};
	CameraManager->StartCameraFade(1, 0, 3, black, false, true);
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
	PlayerInputHandle.GetActionDelegate(0).BindUObject(this, &ADungeonCharacter::Interact);
	PlayerInputHandle.GetActionDelegate(1).BindUObject(this, &ADungeonCharacter::Attack);
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

void ADungeonCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void ADungeonCharacter::MoveForward(const float _value)
{	
	if (Controller && _value != 0.0f)
	{
		//const FRotator _rotation = Controller->GetControlRotation();
		//const FRotator _yawRot(0.f, _rotation.Yaw, 0.f);
		//const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::X);
		
		const FRotator _cameraDir = CameraManager->GetCameraRotation();
		const FRotator _yawRot(0.f, _cameraDir.Yaw, 0.f);
		const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::X);
		
		//UE_LOG(LogTemp, Warning, TEXT("Forward: %f"), _direction.X);
		
		AddMovementInput(_direction, _value);
	}
}

void ADungeonCharacter::MoveRight(const float _value)
{	
	if (Controller && _value != 0.0f)
	{
		//const FRotator _rotation = Controller->GetControlRotation();
		//const FRotator _yawRot(0.f, _rotation.Yaw, 0.f);
		//const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::Y);

		const FRotator _cameraDir = CameraManager->GetCameraRotation();
		const FRotator _yawRot(0.f, _cameraDir.Yaw, 0.f);
		const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::Y);
		
		//UE_LOG(LogTemp, Warning, TEXT("Right  : %f"), _direction.Y);

		AddMovementInput(_direction, _value);
	}
}



void ADungeonCharacter::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractable* interactable = Cast<IInteractable>(OtherActor);
	if (interactable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something has entered Collision!"));
        FInputActionHandlerSignature signature; signature.BindRaw(interactable, &IInteractable::Interact_Implementation);
        PlayerInputHandle.GetActionData(0).Push(signature);
        PlayerInputHandle.SetDelegateToAction(0);
	}
	
}


void ADungeonCharacter::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Something has exited Collision!"));
	//PlayerInputHandle.GetActionDelegate(0).BindUObject(this, &ADungeonCharacter::Interact);
	PlayerInputHandle.GetActionData(0).Pop();
	PlayerInputHandle.SetDelegateToAction(0);
}

void ADungeonCharacter::ActivateCharacter()
{
	//EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}

void ADungeonCharacter::DeactivateCharacter()
{
	//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}



