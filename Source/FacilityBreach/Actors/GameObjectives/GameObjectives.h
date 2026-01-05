#pragma once

#include "CoreMinimal.h"
#include "GameObjectives.generated.h"

UENUM(BlueprintType)
enum class EGameObjectiveGoalAction : uint8
{
	GENERAL_Interact = 0,

	DOOR_Open = 10,
	DOOR_Close = 11,

	NONE = 255
};

USTRUCT(BlueprintType)
struct FGameObjectiveGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ID;

	UPROPERTY(EditAnywhere)
	FText Title = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
	TArray<EGameObjectiveGoalAction> Actions;

	UPROPERTY(EditAnywhere)
	TArray<FName> ActorTags;

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
