// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/TriggerBox.h"
#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "FacilityBreach/Subsystems/World/CheckPointsSubsystem.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "FacilityBreach/Subsystems/World/TutorialSubsystem.h"
#include "CheckPoint.generated.h"

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class UCheckPointTrigger : public UObject
{
	GENERATED_BODY()

public:

	virtual void Execute();
	
};

UCLASS()
class UCheckPointTrigger_Tutorial : public UCheckPointTrigger
{
	GENERATED_BODY()

public:

	virtual void Execute() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTutorial> Tutorial;
};

UCLASS()
class UCheckPointTrigger_GameObjective : public UCheckPointTrigger
{
	GENERATED_BODY()

public:

	virtual void Execute() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGameObjective> GameObjective;
};

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API ACheckPoint : public ATriggerBox, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	/* IGameplayTagAssetInterface */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/* END IGameplayTagAssetInterface */
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Trigger", meta=(ToolTip="Wether to fire triggers once or multiple times"))
	bool bFireOnce = true;

	UPROPERTY(EditAnywhere, Instanced, Category="Trigger")
	TArray<TObjectPtr<UCheckPointTrigger>> Triggers;

	/** Gameplay Tags */
	UPROPERTY(Category="Gameplay Tags", EditAnywhere)
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, Category="Trigger", meta=(ToolTip="Wether the player can respawn on this checkpoint"))
	bool bRespawnCheckPoint = false;

private:

	bool bFired = false;

	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UGameObjectivesSubsystem* GameObjectivesSubsystem;
	UCheckPointsSubsystem* CheckPointsSubsystem;
	
};
