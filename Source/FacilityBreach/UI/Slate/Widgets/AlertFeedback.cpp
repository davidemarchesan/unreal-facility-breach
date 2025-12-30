// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertFeedback.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/Layout/SScaleBox.h"

void SAlertFeedback::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		.StretchDirection(EStretchDirection::DownOnly)
		[
			SAssignNew(Image, SImage)
			.ColorAndOpacity(FLinearColor::Yellow)
			.Image(FFacilityBreachStyle::Get().GetBrush("General.Awareness.Icon"))
		]
	];
}

void SAlertFeedback::SetIcon(const FName InIcon, const TAttribute<FSlateColor>& InColor)
{
	if (Image)
	{
		Image->SetImage(FFacilityBreachStyle::Get().GetBrush(InIcon));
		Image->SetColorAndOpacity(InColor);
	}
}
