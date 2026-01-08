// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TutorialSubsystem.generated.h"

UCLASS()
class FACILITYBREACH_API UTutorial : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title = FText::GetEmpty();;

	UPROPERTY(EditAnywhere)
	FText Description = FText::GetEmpty();;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShowTutorial, const FText&, const FText&);
DECLARE_MULTICAST_DELEGATE(FOnHideTutorial);

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UTutorialSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	void ShowTutorial(const FText& Title, const FText& Description);
	void HideTutorial();

	FOnShowTutorial OnTutorialShow;
	FOnHideTutorial OnTutorialHide;
	
};
