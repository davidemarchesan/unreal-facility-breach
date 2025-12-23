// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"

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

	bool bWayPointsRegistered = BuildWayPointLocations();

	if (bWayPointsRegistered)
	{
		GoToNextWayPoint();
	}

	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//
	// AController* C = GetController();
	// UE_LOG(LogTemp, Warning, TEXT("Controller class: %s"), *GetNameSafe(C));
	//
	// AAIController* AIController = Cast<AAIController>(GetController());

	// FVector Target = GetActorLocation() + FVector(300, 0, 0);
	// EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(Target);
	// UE_LOG(LogTemp, Warning, TEXT("Classic Result %d"), Result);

	// if (AIController)
	// {
	// 	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	// 	FNavLocation NavLocation;
	//
	// 	if (NavSystem && NavSystem->ProjectPointToNavigation(FVector(1315.f, 647.f, 1.f), NavLocation))
	// 	{
	// 		EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(NavLocation.Location);
	// 		UE_LOG(LogTemp, Warning, TEXT("Nav Result %d | Location %f, %f, %f"), Result, NavLocation.Location.X,
	// 		       NavLocation.Location.Y, NavLocation.Location.Z);
	// 	}
	// 	else
	// 	{
	// 		FVector Target = GetActorLocation() + FVector(300, 0, 0);
	// 		EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(Target);
	// 		UE_LOG(LogTemp, Warning, TEXT("Classic Result %d"), Result);
	// 	}
	// }
}

UNavigationSystemV1* AEnemyGuardCharacter::GetNavSystem()
{
	if (CachedNavSystem)
	{
		return CachedNavSystem;
	}

	CachedNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	return CachedNavSystem;
}

AAIController* AEnemyGuardCharacter::GetAIController()
{
	if (CachedAIController)
	{
		return CachedAIController;
	}

	CachedAIController = Cast<AAIController>(GetController());

	if (CachedAIController)
	{
		CachedAIController->ReceiveMoveCompleted.AddDynamic(this, &AEnemyGuardCharacter::OnMoveCompleted);
	}
	
	return CachedAIController;
}

void AEnemyGuardCharacter::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	GoToNextWayPoint();
}

bool AEnemyGuardCharacter::BuildWayPointLocations()
{
	if (WayPoints.IsEmpty())
	{
		return false;
	}

	UNavigationSystemV1* NavSystem = GetNavSystem();

	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardCharacter::BuildWayPointLocations() Could not find a NavSystem"));
		return false;
	}

	for (int32 i = 0; i < WayPoints.Num(); i++)
	{
		if (WayPoints[i] != nullptr)
		{
			FNavLocation OutNavLocation;
			if (NavSystem->ProjectPointToNavigation(WayPoints[i]->GetActorLocation(), OutNavLocation))
			{
				WayPointLocations.Add(OutNavLocation.Location);
			}
		}
	}

	return WayPointLocations.IsEmpty() == false;
}

void AEnemyGuardCharacter::GoToNextWayPoint()
{
	CurrentWayPointIndex++;

	if (WayPointLocations.IsValidIndex(CurrentWayPointIndex) == false)
	{
		CurrentWayPointIndex = 0;
	}

	CurrentWayPointLocation = WayPointLocations[CurrentWayPointIndex];

	if (AAIController* AIController = GetAIController())
	{
		EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(CurrentWayPointLocation);

		if (Result == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController->MoveToLocation Failed for location %f, %f, %f"),
			       CurrentWayPointLocation.X,
			       CurrentWayPointLocation.Y, CurrentWayPointLocation.Z);
		}
	}
}

void AEnemyGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyGuardCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Warning, TEXT("possessed by"));
}
