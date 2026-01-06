// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectivesSubsystem.h"

#include "GameplayTagAssetInterface.h"
#include "FacilityBreach/Actors/GameObjectives/GameObjectivesManager.h"
#include "Kismet/GameplayStatics.h"

void UGameObjectivesSubsystem::LoadGameObjectives()
{
	if (AGameObjectivesManager* GameObjectivesManager = Cast<AGameObjectivesManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGameObjectivesManager::StaticClass())))
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

void UGameObjectivesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UGameplayTagsManager* GameplayTagsManager = UGameplayTagsManager::GetIfAllocated())
	{
		Tag_Action_GeneralInteract = GameplayTagsManager->RequestGameplayTag("Action.General.Interact");
		Tag_Action_DoorOpen = GameplayTagsManager->RequestGameplayTag("Action.Door.Open");
		Tag_Action_DoorClose = GameplayTagsManager->RequestGameplayTag("Action.Door.Close");
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
			// Separating data vs status
			TArray<FGameObjectiveGoalState> GoalStates;

			for (FGameObjectiveGoal& Goal : GameObjective->Goals)
			{
				GoalStates.Add(FGameObjectiveGoalState(Goal.ID, Goal.Title, Goal.ActionGameplayTags,
				                                       Goal.ActorGameplayTags, Goal.Count));
			}

			CurrentObjectiveState = FGameObjectiveState(GameObjective->ID, GoalStates);
			CurrentObjectiveState.bActive = true;

			UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: set a new objective"));

			// Update UI
			OnGameObjectiveUpdate.Broadcast(CurrentObjectiveState);
		}
	}
}

void UGameObjectivesSubsystem::Emit(AActor* Actor, FGameplayTag ActionGameplayTag)
{
	FString Tags = "";

	if (Actor == nullptr)
	{
		return;
	}

	if (Actor->Implements<UGameplayTagAssetInterface>() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGameObjectivesSubsystem: Actor does not implement the gameplay tag interface"));
		return;
	}

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

	// Wether to update UI
	bool bUpdateUI = false;
	bool bLeastOneGoalCompleted = false;

	TScriptInterface<IGameplayTagAssetInterface> TagInterface = TScriptInterface<
		IGameplayTagAssetInterface>(Actor);

	if (TagInterface)
	{
		for (FGameObjectiveGoalState& Goal : CurrentObjectiveState.Goals)
		{
			if (Goal.ActionGameplayTags.HasTagExact(ActionGameplayTag) == false)
			{
				// The action is not relevant for this goal
				continue;
			}

			for (FGameplayTag RequiredTag : Goal.ActorGameplayTags)
			{
				FGameplayTagContainer OutActorGameplayTags;
				TagInterface->GetOwnedGameplayTags(OutActorGameplayTags);

				if (OutActorGameplayTags.HasTagExact(RequiredTag))
				{
					Goal.CurrentCount++;
					bUpdateUI = true; // A goal has changed, update UI

					if (Goal.CurrentCount == Goal.Count)
					{
						Goal.bCompleted = true;
						bLeastOneGoalCompleted = true;
					}

					break;
				}
			}
		}
	}

	// Check if all goals are completed
	bool bAllGoalsCompleted = true;
	for (FGameObjectiveGoalState& Goal : CurrentObjectiveState.Goals)
	{
		if (Goal.bCompleted == false)
		{
			bAllGoalsCompleted = false;
			break;
		}
	}

	if (bAllGoalsCompleted == true)
	{
		CurrentObjectiveState.bCompleted = true;
		OnGameObjectiveCompleted.Broadcast(CurrentObjectiveState);
	} else if (bLeastOneGoalCompleted == true)
	{
		OnGameObjectiveGoalCompleted.Broadcast(CurrentObjectiveState);
	}

	if (bUpdateUI == true)
	{
		OnGameObjectiveUpdate.Broadcast(CurrentObjectiveState);
	}
}
