// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCamera.h"

// Sets default values
ADungeonCamera::ADungeonCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	

	Camera->SetupAttachment(SpringArm, SpringArm->SocketName);

	SpringArm->SetRelativeRotation(FRotator(0.f, -45.f, -45.f));
	SpringArm->TargetArmLength = 4000.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 1.0f;

	SpringArm->bInheritPitch =
		SpringArm->bInheritRoll =
			SpringArm->bInheritYaw = false;

	TargetPoint = {0,0,0};
}

// Called when the game starts or when spawned
void ADungeonCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ADungeonCamera::AttachToActor(AActor* _other)
{
	
}

void ADungeonCamera::DetachFromActor(AActor* _other)
{
	
}

