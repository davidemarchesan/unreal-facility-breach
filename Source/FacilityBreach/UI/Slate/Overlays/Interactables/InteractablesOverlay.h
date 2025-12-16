// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	void OnInteractableFocus(FText Hint);
	void OnInteractableFocusEnd();

private:

	TSharedPtr<SOverlay> RootOverlay;
	TSharedPtr<STextBlock> HintTextBlock;
};
