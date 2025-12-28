// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardCharacter.h"
#include "Components/CapsuleComponent.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"

AEnemyGuardCharacter::AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

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

	if (SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGuardCharacter::OnComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyGuardCharacter::OnComponentEndOverlap);
	}
}

void AEnemyGuardCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardCharacter::OnComponentBeginOverlap"));
	Player = Cast<AFirstPersonCharacter>(OtherActor);
}

void AEnemyGuardCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardCharacter::OnComponentEndOverlap"));
	Player = nullptr;
}

void AEnemyGuardCharacter::CheckPlayerPosition()
{

	if (Player == nullptr)
	{
		return;
	}
	
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();

		FVector Direction = PlayerLocation - GetActorLocation();
		Direction.Normalize();

		const float DotProduct = FVector::DotProduct(GetActorForwardVector(), Direction);

		if (DotProduct >= 0)
		{
			// Between 0 and 90 degrees
			// Now check if it's in vision range
			float Cos = FMath::Cos(FMath::DegreesToRadians(VisionAngle * 0.5f));

			if (DotProduct >= Cos)
			{
				UE_LOG(LogTemp, Warning, TEXT("I CAN SEE YOU"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("cannot see you :("));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("you are at my back :("));
		}
	}
}

void AEnemyGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugCone(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector(),
		1000.f,
		FMath::DegreesToRadians(VisionAngle),
		FMath::DegreesToRadians(VisionAngle),
		4,
		FColor::Red
	);

	if (Player)
	{
		CheckPlayerPosition();
	}
}

// Called to bind functionality to input
void AEnemyGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
