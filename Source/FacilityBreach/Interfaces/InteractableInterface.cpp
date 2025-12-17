// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

bool IInteractableInterface::IsInteractable()
{
	return false;
}

FText IInteractableInterface::GetHint()
{
	return FText::FromString("Interactable");
}

void IInteractableInterface::OnInteract(APawn* PawnInstigator)
{
	return;
}

void IInteractableInterface::OnFocus(APawn* PawnInstigator)
{
	return;
}