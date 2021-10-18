// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCharacter.h"

#include "DungeonDoor.h"
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
	
	// Set data to the Interaction Capsule Component, such as its size, profile name ecc.
	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interaction Capsule Collider"));
	InteractionCapsule->InitCapsuleSize(35, 100);
	InteractionCapsule->AddRelativeLocation({100, 0, 0});
	InteractionCapsule->SetCollisionProfileName(TEXT("Interaction"));
	InteractionCapsule->SetupAttachment(GetCapsuleComponent());
	InteractionCapsule->SetGenerateOverlapEvents(true);
	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionBeginOverlap);
	InteractionCapsule->OnComponentEndOverlap.AddDynamic(this, &ADungeonCharacter::OnInteractionEndOverlap);
	 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SetRelativeRotation(FRotator(-45.f, -45.f, 0.f));
	//SpringArm->TargetArmLength = 4000.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bInheritPitch =
		SpringArm->bInheritRoll =
			SpringArm->bInheritYaw = false;
	SpringArm->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ADungeonCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation({0,0,100});
	
	if (GEngine && !CameraManager)
	{
		CameraManager = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
	}
	FLinearColor Black{0.0f, 0.0f, 0.0f, 0.0f};
	CameraManager->StartCameraFade(1, 0, 3, Black, false, true);
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
	// Registration of new Actions.
	PlayerInputHandle.RegisterNewAction(TEXT("Interaction"), EInputEvent::IE_Pressed);
	PlayerInputHandle.RegisterNewAction(TEXT("Attack"), EInputEvent::IE_Pressed);
	// Binding a function to each Action's delegate.
	PlayerInputHandle.GetActionDelegate(0).BindUObject(this, &ADungeonCharacter::Interact);
	PlayerInputHandle.GetActionDelegate(1).BindUObject(this, &ADungeonCharacter::Attack);
	// Set each Delegate to its proper Action.
	PlayerInputHandle.SetDelegateToAction(0);
	PlayerInputHandle.SetDelegateToAction(1);
	
	//PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Interact);
	//PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ADungeonCharacter::Attack);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADungeonCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADungeonCharacter::MoveRight);
	
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

		// Get direction by the Camera direction
		const FRotator CameraDir = CameraManager->GetCameraRotation();
		const FRotator YawRot(0.f, CameraDir.Yaw, 0.f);
		const FVector  Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
				
		AddMovementInput(Direction, _value);
	}
}

void ADungeonCharacter::MoveRight(const float _value)
{	
	if (Controller && _value != 0.0f)
	{
		//const FRotator _rotation = Controller->GetControlRotation();
		//const FRotator _yawRot(0.f, _rotation.Yaw, 0.f);
		//const FVector _direction = FRotationMatrix(_yawRot).GetUnitAxis(EAxis::Y);

		// Get direction by the Camera direction
		const FRotator CameraDir = CameraManager->GetCameraRotation();
		const FRotator YawRot(0.f, CameraDir.Yaw, 0.f);
		const FVector  Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(Direction, _value);
	}
}


void ADungeonCharacter::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractable* Interactable = Cast<IInteractable>(OtherActor);
	if (Interactable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something has entered Collision!"));
        FInputActionHandlerSignature Signature;
		Signature.BindRaw(Interactable, &IInteractable::Interact_Implementation);
        PlayerInputHandle.GetActionData(0).Push(Signature);
        PlayerInputHandle.SetDelegateToAction(0);
	}
	
}

void ADungeonCharacter::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractable* Interactable = Cast<IInteractable>(OtherActor);
	if (Interactable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Something has exited Collision!"));
		//PlayerInputHandle.GetActionDelegate(0).BindUObject(this, &ADungeonCharacter::Interact);
		PlayerInputHandle.GetActionData(0).Pop();
		PlayerInputHandle.SetDelegateToAction(0);
	}
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



