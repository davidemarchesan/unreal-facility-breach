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
		bLoopWayPoints = EnemyGuard->GetLoopWayPoints();

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
	int32 NextWayPointIndex = CurrentWayPointIndex + WayPointOrder;

	if (WayPointLocations.IsValidIndex(NextWayPointIndex))
	{
		CurrentWayPointIndex = NextWayPointIndex;
	}
	else
	{
		if (bLoopWayPoints == true)
		{
			CurrentWayPointIndex = 0;
		}
		else
		{
			if (NextWayPointIndex < 0)
			{
				// Next waypoint was below zero, go back from first to last
				WayPointOrder = 1;
			}

			if (NextWayPointIndex >= WayPointLocations.Num())
			{
				// Next waypoint was above max, go back from last to first
				WayPointOrder = -1;
			}

			NextWayPointIndex = CurrentWayPointIndex + WayPointOrder;
			if (WayPointLocations.IsValidIndex(NextWayPointIndex))
			{
				CurrentWayPointIndex = NextWayPointIndex;
			}
			else
			{
				return;
			}
		}
	}

	const FVector NextWayPointLocation = WayPointLocations[CurrentWayPointIndex];

	EPathFollowingRequestResult::Type Result = MoveToLocation(NextWayPointLocation);

	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardAIController::GoToNextWayPoint() Failed for location %f, %f, %f"),
		       NextWayPointLocation.X,
		       NextWayPointLocation.Y, NextWayPointLocation.Z);
	}
}
