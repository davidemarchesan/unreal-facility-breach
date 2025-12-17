// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SInteractablesOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInteractablesOverlay)
		{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	void OnShowInteractionHint(FInteractionHint Hint);
	void OnHideInteractionHint();

private:

	TSharedPtr<SOverlay> RootOverlay;
	TSharedPtr<STextBlock> HintTextBlock;
	TSharedPtr<SBox> InputBox;
};
