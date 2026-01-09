// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardAIController.h"

#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyGuardAIController::StartPatrol(TArray<TObjectPtr<AWayPoint>> InWayPoints, bool bInLoopWayPoints)
{
	if (BuildWayPointLocations(InWayPoints))
	{
		bLoopWayPoints = bInLoopWayPoints;
	}

	EnterPatrol();
}

void AEnemyGuardAIController::OnTargetSeen(AActor* Target)
{
	bPlayerInVision = true;
	TargetActor = Target;

	if (State == EAIGuardState::STATE_Patrol)
	{
		EnterSuspicious();
	}
}

void AEnemyGuardAIController::OnTargetLost()
{
	bPlayerInVision = false;

	if (TargetActor)
	{
		LastKnownTargetLocation = TargetActor->GetActorLocation();
	}

	if (State == EAIGuardState::STATE_Chase)
	{
		EnterAlert();
	}
}

void AEnemyGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (State == EAIGuardState::STATE_Patrol)
	{
		OnPatrolCompleted();
	}

	if (State == EAIGuardState::STATE_Alert)
	{
		EnterSuspicious();
	}
}

void AEnemyGuardAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitialLocation = InPawn->GetActorLocation();
	InitialRotation = InPawn->GetActorRotation();
}

bool AEnemyGuardAIController::BuildWayPointLocations(TArray<TObjectPtr<AWayPoint>> InWayPoints)
{
	WayPoints.Empty();
	WayPoints = InWayPoints;

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

void AEnemyGuardAIController::MoveToWayPoint()
{
	if (WayPoints.IsValidIndex(CurrentWayPointIndex))
	{
		const FVector CurrentWayPointLocation = WayPointLocations[CurrentWayPointIndex];
		EPathFollowingRequestResult::Type Result = MoveToLocation(CurrentWayPointLocation);

		if (Result == EPathFollowingRequestResult::Type::Failed)
		{
			UE_LOG(LogTemp, Warning, TEXT("AEnemyGuardAIController::GoToNextWayPoint() Failed for location %f, %f, %f"),
			       CurrentWayPointLocation.X,
			       CurrentWayPointLocation.Y, CurrentWayPointLocation.Z);
		}
	}
}

void AEnemyGuardAIController::MoveToInitialLocation()
{
	EPathFollowingRequestResult::Type Result = MoveToLocation(InitialLocation);

	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AEnemyGuardAIController::MoveToInitialLocation() Failed for location %f, %f, %f"),
		       InitialLocation.X,
		       InitialLocation.Y, InitialLocation.Z);
	}
}

void AEnemyGuardAIController::EnterPatrol()
{
	SetState(EAIGuardState::STATE_Patrol);

	if (WayPoints.IsEmpty())
	{
		MoveToInitialLocation();
	}
	else
	{
		MoveToWayPoint();
	}

	OnEnterPatrol.Broadcast();
}

void AEnemyGuardAIController::OnPatrolCompleted()
{

	if (WayPoints.IsEmpty())
	{
		// Means the AI returned to initial location, set initial rotation
		if (APawn* ControlledActor = GetPawn())
		{
			ControlledActor->SetActorRotation(InitialRotation);
		}
		return;
	}
	
	if (WayPoints.IsValidIndex(CurrentWayPointIndex))
	{
		AWayPoint* CurrentWayPoint = WayPoints[CurrentWayPointIndex];

		if (CurrentWayPoint && CurrentWayPoint->StayInPositionTime > 0.f)
		{
			// Move when timer finishes
			GetWorldTimerManager().ClearTimer(WayPointStayTimerHandle);
			GetWorldTimerManager().SetTimer(
				WayPointStayTimerHandle,
				this,
				&AEnemyGuardAIController::NextWayPoint,
				CurrentWayPoint->StayInPositionTime,
				false
			);

			// Do we need to face the same WayPoint direction
			if (CurrentWayPoint->bFaceWayPointDirection == true)
			{
				if (APawn* ControlledActor = GetPawn())
				{
					ControlledActor->SetActorRotation(CurrentWayPoint->GetActorRotation());
				}
			}

			return;
		}
	}

	NextWayPoint();
}

void AEnemyGuardAIController::NextWayPoint()
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

	MoveToWayPoint();
}

void AEnemyGuardAIController::EnterSuspicious()
{
	SetState(EAIGuardState::STATE_Suspicious);
	StopMovement();

	GetWorldTimerManager().ClearTimer(SuspiciousTimerHandle);
	GetWorldTimerManager().SetTimer(
		SuspiciousTimerHandle,
		this,
		&AEnemyGuardAIController::CheckSuspiciousness,
		1.f,
		false
	);

	OnEnterSuspicious.Broadcast();
}

void AEnemyGuardAIController::CheckSuspiciousness()
{
	if (bPlayerInVision == true)
	{
		EnterChase();
	}
	else
	{
		EnterPatrol();
	}
}

void AEnemyGuardAIController::EnterChase()
{
	if (TargetActor == nullptr)
	{
		return;
	}

	SetState(EAIGuardState::STATE_Chase);
	MoveToActor(TargetActor);

	OnEnterChase.Broadcast();
}

void AEnemyGuardAIController::EnterAlert()
{
	if (TargetActor)
	{
		StopMovement();
		SetState(EAIGuardState::STATE_Alert);
		MoveToLocation(LastKnownTargetLocation); // Last known position
	}
}
