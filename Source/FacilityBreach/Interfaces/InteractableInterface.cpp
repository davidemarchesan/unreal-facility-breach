// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

bool IInteractableInterface::IsInteractable()
{
	return false;
}

FInteractionHint IInteractableInterface::GetHint(APawn* PawnInstigator)
{
	return FInteractionHint();
}

void IInteractableInterface::OnInteract(APawn* PawnInstigator)
{
	return;
}

void IInteractableInterface::OnFocus(APawn* PawnInstigator)
{
	return;
}