#pragma once

#include "CoreMinimal.h"
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
	TArray<FName> Actions;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Required tags that Actor needs to have to satisfy this goal"))
	TArray<FName> ActorTags;

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
	TArray<FGameObjectiveGoal> Goals;
};

USTRUCT(BlueprintType)
struct FGameObjectiveGoalState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	FText Title = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
	TArray<FName> Actions;

	UPROPERTY(EditAnywhere)
	TArray<FName> ActorTags;

	UPROPERTY(EditAnywhere)
	int32 Count = 1;

	UPROPERTY(EditAnywhere)
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere)
	bool bCompleted = false;

	// Default constructor
	FGameObjectiveGoalState() = default;

	FGameObjectiveGoalState(FName InID, const FText& InTitle, const TArray<FName>& InActions, const TArray<FName>& InActorTags, const int32 InCount)
		: ID(InID)
		  , Title(InTitle)
		  , Actions(InActions)
		  , ActorTags(InActorTags)
		  , Count(InCount)
	{
	}
};

USTRUCT(BlueprintType)
struct FGameObjectiveState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	TArray<FGameObjectiveGoalState> Goals;

	UPROPERTY(EditAnywhere)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere)
	bool bActive = false;

	// Constructor
	FGameObjectiveState() = default;

	FGameObjectiveState(const FName InID, const TArray<FGameObjectiveGoalState>& InGoals)
		: ID(InID)
		  , Goals(InGoals)
	{
	}
};
