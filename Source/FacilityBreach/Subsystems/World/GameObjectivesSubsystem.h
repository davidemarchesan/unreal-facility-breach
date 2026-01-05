// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameObjectivesSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UGameObjectivesSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void PostInitialize() override;

	void SetGameObjective(FName ID);

	void Emit(AActor* Actor, EGameObjectiveGoalAction Action);

	

protected:

private:

	FGameObjectiveState CurrentObjectiveState;

	TArray<UGameObjective*> GameObjectives;

	void LoadGameObjectives();
	
};
