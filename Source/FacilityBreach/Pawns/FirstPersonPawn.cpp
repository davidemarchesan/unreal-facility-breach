// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"

// Sets default values
AFirstPersonPawn::AFirstPersonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("CollisionComponent"));
	RootComponent = CollisionComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLocation(DeltaTime);
}

// Called to bind functionality to input
void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFirstPersonPawn::RequestMove(FVector2D InDirection)
{
	InDirection.Normalize();
	MovementDirection = InDirection;
}

void AFirstPersonPawn::UpdateLocation(float DeltaTime)
{

	if (MovementDirection.IsNearlyZero())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UpdateLocation: (X %f, Y %f)"), MovementDirection.X, MovementDirection.Y);

	AddMovementInput(GetActorForwardVector(), MovementDirection.X);
	AddMovementInput(GetActorRightVector(), MovementDirection.Y);
	
}

