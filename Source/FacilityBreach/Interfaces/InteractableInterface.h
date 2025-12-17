// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.generated.h"

USTRUCT(BlueprintType)
struct FInteractionHint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Text = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
	bool bShowInput = false;

	FInteractionHint() = default;

	FInteractionHint(const FText& InText, const bool bInShowInput = true)
	{
		Text = InText;
		bShowInput = bInShowInput;
	}
};

/*
Empty class for reflection system visibility.
Uses the UINTERFACE macro.
Inherits from UInterface.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/* Actual Interface declaration. */
class IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual bool IsInteractable();
	virtual FInteractionHint GetHint(APlayerController* PlayerController);

	virtual void OnInteract(APlayerController* PlayerController);
	virtual void OnFocus(APlayerController* PlayerController);
};
