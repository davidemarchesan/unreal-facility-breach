// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardAIController.h"

#include "NavigationSystem.h"
#include "FacilityBreach/Pawns/EnemyGuardCharacter.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyGuardAIController::StartPatrol()
{
	AEnemyGuardCharacter* EnemyGuard = Cast<AEnemyGuardCharacter>(GetCharacter());

	if (EnemyGuard && BuildWayPointLocations(EnemyGuard))
	{
		GoToNextWayPoint();
	}
}

void AEnemyGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	GoToNextWayPoint();
}

void AEnemyGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

bool AEnemyGuardAIController::BuildWayPointLocations(AEnemyGuardCharacter* EnemyGuard)
{
	WayPoints.Empty();
	WayPoints = EnemyGuard->GetWayPoints();
	
	if (WayPoints.IsEmpty())
	{
		return false;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr)
	{
		return false;
	}

	const FVector Extent = FVector(20.f, 20.f, 50.f);

	for (int32 i = 0; i < WayPoints.Num(); i++)
	{
		if (WayPoints[i] != nullptr)
		{
			FNavLocation OutNavLocation;
			
			if (NavSystem->ProjectPointToNavigation(WayPoints[i]->GetActorLocation(), OutNavLocation, Extent))
			{
				WayPointLocations.Add(OutNavLocation.Location);
			}
		}
	}

	return WayPointLocations.IsEmpty() == false;
}

void AEnemyGuardAIController::GoToNextWayPoint()
{
	CurrentWayPointIndex++;

	if (WayPointLocations.IsValidIndex(CurrentWayPointIndex) == false)
	{
		CurrentWayPointIndex = 0;
	}
	
	CurrentWayPointLocation = WayPointLocations[CurrentWayPointIndex];
	
	
	EPathFollowingRequestResult::Type Result = MoveToLocation(CurrentWayPointLocation);
	
	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardAIController::GoToNextWayPoint() Failed for location %f, %f, %f"),
		       CurrentWayPointLocation.X,
		       CurrentWayPointLocation.Y, CurrentWayPointLocation.Z);
	}
}