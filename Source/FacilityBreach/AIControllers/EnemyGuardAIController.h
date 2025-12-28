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

	// In game actors
	TArray<TObjectPtr<AWayPoint>> WayPoints;

	/**
	 * 
	 * @return true if there are any WayPoints
	 */
	bool BuildWayPointLocations(AEnemyGuardCharacter* EnemyGuard);
	TArray<FVector> WayPointLocations;

	int32 CurrentWayPointIndex = 0;

	bool bLoopWayPoints = true;		// true = start from first when on last waypoint; false = go back from last to first when on last waypoint 
	int32 WayPointOrder = 1;		// 1 = from first to last; -1 = from last to first

	void GoToNextWayPoint();
	
};
