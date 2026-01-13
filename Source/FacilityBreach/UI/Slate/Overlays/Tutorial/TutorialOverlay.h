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

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void OnShow(FText InTitle, FText InDescription);
	void OnHide();

private:
	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<STextBlock> TitleTextBlock;
	TSharedPtr<STextBlock> DescriptionTextBlock;

	float FadeOutDelay = 10.f;
	float FadeOutDuration = 1.f;
	float FadeOutStartTime = 0.f;

	bool bIsFading = false;
	bool bFadedOut = false;
	
};
