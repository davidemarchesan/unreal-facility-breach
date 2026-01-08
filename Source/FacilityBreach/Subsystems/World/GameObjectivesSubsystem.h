// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameObjectivesSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameObjectiveNew, const FGameObjectiveState&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameObjectiveUpdate, const FGameObjectiveState&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameObjectiveCompleted, const FGameObjectiveState&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameObjectiveGoalCompleted, const FGameObjectiveState&);

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UGameObjectivesSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void PostInitialize() override;

	void SetGameObjective(FName ID);
	void SetGameObjective(UGameObjective* GameObjective);

	void Emit(AActor* Actor, FGameplayTag ActionGameplayTag);

	FOnGameObjectiveNew OnGameObjectiveNew;
	FOnGameObjectiveUpdate OnGameObjectiveUpdate;
	FOnGameObjectiveCompleted OnGameObjectiveCompleted;
	FOnGameObjectiveGoalCompleted OnGameObjectiveGoalCompleted;

	const FGameObjectiveState& GetCurrentObjectiveState() { return CurrentObjectiveState; }

protected:

private:

	FGameObjectiveState CurrentObjectiveState;

	TArray<UGameObjective*> GameObjectives;

	void LoadGameObjectives();

	void LoadSubsystems();

public:

	/** Action Tags */
	inline static FGameplayTag Tag_Action_GeneralInteract;
	inline static FGameplayTag Tag_Action_DoorOpen;
	inline static FGameplayTag Tag_Action_DoorClose;
	/** End Action Tags */
	
};
