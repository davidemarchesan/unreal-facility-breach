// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FacilityBreach/Actors/WayPoints/WayPoint.h"
#include "FacilityBreach/Pawns/EnemyGuardCharacter.h"
#include "EnemyGuardAIController.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AEnemyGuardAIController : public AAIController
{
	GENERATED_BODY()

public:

	void StartPatrol();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:

	virtual void OnPossess(APawn* InPawn) override;

private:

	TArray<TObjectPtr<AWayPoint>> WayPoints;

	TArray<FVector> WayPointLocations;

	/**
	 * 
	 * @return true if there are any WayPoints
	 */
	bool BuildWayPointLocations(AEnemyGuardCharacter* EnemyGuard);

	int32 CurrentWayPointIndex = 0;
	FVector CurrentWayPointLocation;

	void GoToNextWayPoint();
	
};
