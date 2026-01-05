// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectivesSubsystem.h"

#include "FacilityBreach/Actors/GameObjectives/GameObjectivesManager.h"
#include "Kismet/GameplayStatics.h"

void UGameObjectivesSubsystem::LoadGameObjectives()
{
	if (AGameObjectivesManager* GameObjectivesManager = Cast<AGameObjectivesManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameObjectivesManager::StaticClass())))
	{
		GameObjectives = GameObjectivesManager->GetGameObjectives();

		if (GameObjectives.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UGameObjectivesSubsystem: No Objectives set in Objectives manager"));	
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGameObjectivesSubsystem: Did not found a Game Objectives manager"));
	}
}

void UGameObjectivesSubsystem::PostInitialize()
{
	Super::PostInitialize();

	LoadGameObjectives();

	SetGameObjective("Test.1");
}

void UGameObjectivesSubsystem::SetGameObjective(FName ID)
{
	if (GameObjectives.Num() == 0)
	{
		return;
	}

	for (UGameObjective* GameObjective : GameObjectives)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: SetGameObjective %s"), *GameObjective->ID.ToString());
		if (GameObjective->ID == ID)
		{
			CurrentObjectiveState = FGameObjectiveState(GameObjective->ID, GameObjective->Goals);
			CurrentObjectiveState.bActive = true;
			UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: set a new objective"));
			// todo: update ui
		}
	}
}

void UGameObjectivesSubsystem::Emit(AActor* Actor, FName Action)
{
	FString Tags = "";

	for (FName& Tag : Actor->Tags)
	{
		Tags.Append(Tag.ToString());
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: I received %s action from actor with these tags %s"), *Action.ToString(), *Tags);

	if (CurrentObjectiveState.bActive == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: No current objective active"));
		return;
	}

	if (CurrentObjectiveState.bCompleted == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: Current objective is already completed"));
		return;
	}

	if (CurrentObjectiveState.Goals.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: Current objective has no goals"));
		return;
	}

	
	for (FGameObjectiveGoal& Goal : CurrentObjectiveState.Goals)
	{

		if (Goal.Actions.Contains(Action) == false)
		{
			continue;
		}

		for (FName& RequiredTag : Goal.ActorTags)
		{
			if (Actor->Tags.Contains(RequiredTag))
			{
				Goal.bCompleted = true;
				UE_LOG(LogTemp, Warning, TEXT("Goal %s has been completed"), *Goal.ID.ToString());
				break;
			}
		}
		
	}

	bool bAllGoalsCompleted = true;
	for (FGameObjectiveGoal& Goal : CurrentObjectiveState.Goals)
	{

		if (Goal.bCompleted == false)
		{
			bAllGoalsCompleted = false;
			break;
		}
		
	}

	if (bAllGoalsCompleted == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("All goals completed, objective %s completed"), *CurrentObjectiveState.ID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("objective %s still missing goals"), *CurrentObjectiveState.ID.ToString());
	}
	
}
