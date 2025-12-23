// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Actors/WayPoints/WayPoint.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EnemyGuardCharacter.generated.h"

UCLASS()
class FACILITYBREACH_API AEnemyGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;

	UPROPERTY(Category=AI, EditAnywhere)
	TArray<TObjectPtr<AWayPoint>> WayPoints;

	

private:

	/** AI and NavSystem */
	UNavigationSystemV1* CachedNavSystem;
	UNavigationSystemV1* GetNavSystem();

	AAIController* CachedAIController;
	AAIController* GetAIController();

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	/** WayPoints */
	TArray<FVector> WayPointLocations;

	/**
	 * 
	 * @return true if there are any WayPoints
	 */
	bool BuildWayPointLocations();

	int32 CurrentWayPointIndex = 0;
	FVector CurrentWayPointLocation;

	void GoToNextWayPoint();

};
