// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardCharacter.h"
#include "Components/CapsuleComponent.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"

AEnemyGuardCharacter::AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	DummyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DummyMeshComponent");
	if (DummyMeshComponent)
	{
		DummyMeshComponent->SetupAttachment(GetCapsuleComponent());
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AEnemyGuardAIController* AIController = Cast<AEnemyGuardAIController>(GetController()))
	{
		AIController->StartPatrol();
	}
}

void AEnemyGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugCone(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector(),
		VisionRange,
		FMath::DegreesToRadians(VisionAngleWidth),
		FMath::DegreesToRadians(VisionAngleHeight),
		3,
		FColor::Red
	);
}

// Called to bind functionality to input
void AEnemyGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
