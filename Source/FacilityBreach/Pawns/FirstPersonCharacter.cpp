// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"

#include "Ability/AbilityComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"

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

	InitializePostProcessMaterials();
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
	if (AbilityComponent)
	{
		AbilityComponent->Dash();
	}
}

void AFirstPersonCharacter::Scan()
{
	if (AbilityComponent)
	{
		AbilityComponent->Scan();
	}
}

void AFirstPersonCharacter::InitializePostProcessMaterials()
{
	if (FirstPersonCameraComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializePostProcessMaterials: FirstPersonCameraComponentis not valid"));
		return;
	}

	ScanPostProcessMaterial = UMaterialInstanceDynamic::Create(ScanPostProcessMaterialInstance, this);
	if (ScanPostProcessMaterial == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create post process dynamic material ScanPostProcessMaterial"));
	}

	FirstPersonCameraComponent->PostProcessSettings.WeightedBlendables.Array.Add(
		FWeightedBlendable(1.f, ScanPostProcessMaterial)
	);

	UE_LOG(LogTemp, Warning, TEXT("Should have applied post process material"));

	// if (FirstPersonCameraComponent && ScanPostProcessMaterialInstance)
	// {
	// 	TArray<FPostProcessSettings> OutSettings;
	// 	TArray<float> OutWeights;
	// 	FirstPersonCameraComponent->GetExtraPostProcessBlends(OutSettings, OutWeights);
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("OutSettings %d"), OutSettings.Num());
	//
	// 	for (FPostProcessSettings& Settings : OutSettings)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("adding pp material"));
	// 		Settings.WeightedBlendables.Array.Push(FWeightedBlendable(0.f, ScanPostProcessMaterialInstance));
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("nope"));
	// }
}
