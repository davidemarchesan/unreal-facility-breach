#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "GameObjectives.generated.h"

USTRUCT(BlueprintType)
struct FGameObjectiveGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	FText Title = FText::GetEmpty();

	UPROPERTY(EditAnywhere, meta=(ToolTip="Actions that can complete this goal"))
	FGameplayTagContainer ActionGameplayTags;

	UPROPERTY(EditAnywhere, meta=(ToolTip="List of gameplay tags that Actor should have in order to satisfy this goal"))
	FGameplayTagContainer ActorGameplayTags;

	UPROPERTY(EditAnywhere,
		meta=(ToolTip="How many times does this goal have to be done before being set as completed"))
	int32 Count = 1;

	// Default constructor
	FGameObjectiveGoal() = default;

	FGameObjectiveGoal(FName InID, const FText& InTitle)
		: ID(InID)
		  , Title(InTitle)
	{
	}
};

UCLASS()
class FACILITYBREACH_API UGameObjective : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	FText Title = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
	TArray<FGameObjectiveGoal> Goals;
};

USTRUCT()
struct FGameObjectiveGoalState
{

	GENERATED_BODY()
	
	FName ID;

	FText Title = FText::GetEmpty();

	FGameplayTagContainer ActionGameplayTags;

	FGameplayTagContainer ActorGameplayTags;

	int32 Count = 1;

	int32 CurrentCount = 0;

	bool bCompleted = false;

	// Default constructor
	FGameObjectiveGoalState() = default;

	FGameObjectiveGoalState(FName InID, const FText& InTitle, const FGameplayTagContainer& InActionGameplayTags,
	                        const FGameplayTagContainer& InActorGameplayTags, const int32 InCount)
		: ID(InID)
		  , Title(InTitle)
		  , ActionGameplayTags(InActionGameplayTags)
		  , ActorGameplayTags(InActorGameplayTags)
		  , Count(InCount)
	{
	}
};

USTRUCT()
struct FGameObjectiveState
{

	GENERATED_BODY()
	
	FName ID;

	FText Title = FText::GetEmpty();

	TArray<FGameObjectiveGoalState> Goals;

	bool bCompleted = false;

	bool bActive = false;

	// Constructor
	FGameObjectiveState() = default;

	FGameObjectiveState(const FName InID, const FText& InTitle, const TArray<FGameObjectiveGoalState>& InGoals)
		: ID(InID)
		  , Title(InTitle)
		  , Goals(InGoals)
	{
	}
};


namespace GameObjectiveGoalActions
{
	inline UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	static const FGameplayTag DoorOpen = GameplayTagsManager.RequestGameplayTag("Action.Door.Open");
}
