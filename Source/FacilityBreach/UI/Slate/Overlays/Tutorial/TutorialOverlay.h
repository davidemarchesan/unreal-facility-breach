// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class STutorialOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STutorialOverlay)
		{
		}
		SLATE_ARGUMENT(FText, Title);
		SLATE_ARGUMENT(FText, Description);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void OnShow(FText InTitle, FText InDescription);
	void OnHide();

private:
	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<STextBlock> TitleTextBlock;
	TSharedPtr<STextBlock> DescriptionTextBlock;
};
