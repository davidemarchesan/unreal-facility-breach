// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.generated.h"

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
	virtual FText GetHint(APawn* PawnInstigator);
	
	virtual void OnInteract(APawn* PawnInstigator);
	virtual void OnFocus(APawn* PawnInstigator);
	
};
