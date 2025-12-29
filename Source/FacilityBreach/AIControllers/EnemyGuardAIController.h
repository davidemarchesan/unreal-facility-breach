// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FacilityBreach/Actors/WayPoints/WayPoint.h"
#include "EnemyGuardAIController.generated.h"

UENUM(BlueprintType)
enum class EAIGuardState : uint8
{
	STATE_Idle,
	STATE_Patrol,
	STATE_Suspicious,
	STATE_Chase,
	STATE_Alert
};

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AEnemyGuardAIController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	EAIGuardState State = EAIGuardState::STATE_Idle;

	void StartPatrol(TArray<TObjectPtr<AWayPoint>> InWayPoints, bool bInLoopWayPoints);

	void OnTargetSeen(AActor* Target);
	void OnTargetLost();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:

	virtual void OnPossess(APawn* InPawn) override;

private:

	void SetState(EAIGuardState NewState) { State = NewState; }

	

	// In game actors
	TArray<TObjectPtr<AWayPoint>> WayPoints;

	/**
	 * 
	 * @return true if there are any WayPoints
	 */
	bool BuildWayPointLocations(TArray<TObjectPtr<AWayPoint>> InWayPoints);
	
	TArray<FVector> WayPointLocations;

	int32 CurrentWayPointIndex = 0;

	bool bLoopWayPoints = true;		// true = start from first when on last waypoint; false = go back from last to first when on last waypoint 
	int32 WayPointOrder = 1;		// 1 = from first to last; -1 = from last to first

	TObjectPtr<AActor> TargetActor = nullptr;
	bool bPlayerInVision = false;

	FVector LastKnownTargetLocation;

	/** Patroling */
	void EnterPatrol();
	void MoveToWayPoint();
	void NextWayPoint();

	/** Suspiciouning */
	void EnterSuspicious();
	FTimerHandle SuspiciousTimerHandle;
	void CheckSuspiciousness();

	/** Chasing */
	void EnterChase();
	
	/** Alert */
	void EnterAlert();
	
};
