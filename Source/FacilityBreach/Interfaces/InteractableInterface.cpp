// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

bool IInteractableInterface::IsInteractable()
{
	return false;
}

FInteractionHint IInteractableInterface::GetHint(APlayerController* PlayerController)
{
	return FInteractionHint();
}

void IInteractableInterface::OnInteract(APlayerController* PlayerController)
{
	return;
}

void IInteractableInterface::OnFocus(APlayerController* PlayerController)
{
	return;
}

void IInteractableInterface::OnFocusLost(APlayerController* PlayerController)
{
	return;
}
