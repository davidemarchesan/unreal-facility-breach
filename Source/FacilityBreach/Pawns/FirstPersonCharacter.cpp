// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"

#include "Ability/AbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "FacilityBreach/PlayerStates/FirstPersonPlayerState.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UFirstPersonMovementComponent>(CharacterMovementComponentName)
	)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCameraComponent"));
	FirstPersonCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FirstPersonCameraComponent->bUsePawnControlRotation = true; // Rotate camera's pitch instead of pawn's

	// Ability component
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(FName("AbilityComponent"));
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFirstPersonCharacter::Dash()
{
	UE_LOG(LogTemp, Warning, TEXT("Character: Dash!"));
	if (AbilityComponent)
	{
		AbilityComponent->Dash();
	}
}

void AFirstPersonCharacter::AddItemToInventory(FString ItemName, int32 Quantity)
{
	if (AFirstPersonPlayerState* State = Cast<AFirstPersonPlayerState>(GetPlayerState()))
	{
		if (State->Inventory)
		{
			State->Inventory->AddItem(ItemName, Quantity);
		}
	}
}

bool AFirstPersonCharacter::HasItemInInventory(FString ItemName, int32 QuantityRequired)
{
	if (AFirstPersonPlayerState* State = Cast<AFirstPersonPlayerState>(GetPlayerState()))
	{
		if (State->Inventory)
		{
			return State->Inventory->HasItem(ItemName, QuantityRequired);
		}
	}
	return false;
}
