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
	}
}

void UGameObjectivesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UGameplayTagsManager* GameplayTagsManager = UGameplayTagsManager::GetIfAllocated())
	{
		Tag_Action_GeneralInteract = GameplayTagsManager->RequestGameplayTag("Action.General.Interact");
		Tag_Action_Overlap_Begin = GameplayTagsManager->RequestGameplayTag("Action.Overlap.Begin");
		Tag_Action_DoorOpen = GameplayTagsManager->RequestGameplayTag("Action.Door.Open");
		Tag_Action_DoorClose = GameplayTagsManager->RequestGameplayTag("Action.Door.Close");
		Tag_Action_Item_PickUp = GameplayTagsManager->RequestGameplayTag("Action.Item.PickUp");
		Tag_Action_Ability_Scan = GameplayTagsManager->RequestGameplayTag("Action.Ability.Scan");
		Tag_Action_Ability_Dash = GameplayTagsManager->RequestGameplayTag("Action.Ability.Dash");
		Tag_Action_Input_Jump = GameplayTagsManager->RequestGameplayTag("Action.Input.Jump");
		Tag_Action_Enemy_Deactivated = GameplayTagsManager->RequestGameplayTag("Action.Enemy.Deactivated");
	}
}

void UGameObjectivesSubsystem::PostInitialize()
{
	Super::PostInitialize();

	LoadGameObjectives();
	
}

void UGameObjectivesSubsystem::SetGameObjective(FName ID)
{
	if (GameObjectives.Num() == 0)
	{
		return;
	}

	for (UGameObjective* GameObjective : GameObjectives)
	{
		if (GameObjective->ID == ID)
		{
			SetGameObjective(GameObjective);
			break;
		}
	}
}

void UGameObjectivesSubsystem::SetGameObjective(UGameObjective* GameObjective)
{
	// Separating data vs status
	TArray<FGameObjectiveGoalState> GoalStates;

	for (FGameObjectiveGoal& Goal : GameObjective->Goals)
	{
		GoalStates.Add(FGameObjectiveGoalState(Goal.ID, Goal.Title, Goal.ActionGameplayTags,
		                                       Goal.ActorGameplayTags, Goal.Count));
	}

	CurrentObjectiveState = FGameObjectiveState(GameObjective->ID, GameObjective->Title, GoalStates);
	CurrentObjectiveState.bActive = true;

	// Update UI
	OnGameObjectiveNew.Broadcast(CurrentObjectiveState);
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
	}
	else if (bLeastOneGoalCompleted == true)
	{
		OnGameObjectiveGoalCompleted.Broadcast(CurrentObjectiveState);
	}

	if (bUpdateUI == true && bAllGoalsCompleted == false)
	{
		OnGameObjectiveUpdate.Broadcast(CurrentObjectiveState);
	}
}
