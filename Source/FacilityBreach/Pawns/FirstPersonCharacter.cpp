// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"

#include "EnemyGuardCharacter.h"
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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	if (AudioComponent)
	{
		AudioComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->OnComponentHit.AddDynamic(this, &AFirstPersonCharacter::OnComponentHit);
	}
}

void AFirstPersonCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (bDead == true)
	{
		return;
	}
	
	if (AEnemyGuardCharacter* Enemy = Cast<AEnemyGuardCharacter>(OtherActor))
	{
		Die();
	}
}

void AFirstPersonCharacter::Die()
{
	bDead = true;
	OnDeath.Broadcast();
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
