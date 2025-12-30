// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SAlertFeedback : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SAlertFeedback) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetIcon(const FName InIcon, const TAttribute<FSlateColor>& InColor);

private:

	TSharedPtr<SImage> Image;
	
};
