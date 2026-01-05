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

	UPROPERTY(EditAnywhere, meta=(ToolTip="How many times does this goal have to be done before being set as completed"))
	int32 Count = 1;

	UPROPERTY(EditAnywhere)
	bool bCompleted = false;

	void SetCompleted(bool bInCompleted = true)
	{
		bCompleted = bInCompleted;
	}

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
struct FGameObjectiveState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	TArray<FGameObjectiveGoal> Goals;

	UPROPERTY(EditAnywhere)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere)
	bool bActive = false;

	// Constructor
	FGameObjectiveState() = default;

	FGameObjectiveState(const FName InID, const TArray<FGameObjectiveGoal>& InGoals)
		: ID(InID)
		  , Goals(InGoals)
	{
	}
};
