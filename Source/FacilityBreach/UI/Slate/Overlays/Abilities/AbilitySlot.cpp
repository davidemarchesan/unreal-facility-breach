// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySlot.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/Layout/SScaleBox.h"

void SAbilitySlot::Construct(const FArguments& InArgs)
{
	const float BoxWidth = 70.f;
	const float BorderRadius = 5.f;

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(BoxWidth)
		.WidthOverride(BoxWidth)
		[

			SNew(SOverlay)

			// Ability image
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[

				SNew(SBox)
				.Padding(10.f)
				[
					SNew(SScaleBox)
					.Stretch(EStretch::ScaleToFit)
					.StretchDirection(EStretchDirection::DownOnly)
					[

						SNew(SImage)
						.Image(FFacilityBreachStyle::Get().GetBrush("Ability.Dash.Icon"))
					]
				]
			]

			// Vertically charging shroud
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SNew(SBox)
				.WidthOverride(BoxWidth)
				.HeightOverride(BoxWidth / 2)
				[
					SNew(SBorder)
					.BorderImage(
						new FSlateRoundedBoxBrush(FLinearColor(1.f, 1.f, 1.f, 0.5f), BorderRadius))
				]
			]

			// Ability disabled shroud
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.5f), BorderRadius))
			]

			// Charges
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(5.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("2"))
				.ColorAndOpacity(FLinearColor::White)
			]

			// Cooldown
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("10"))
				.ColorAndOpacity(FLinearColor::White)
			]


		]
	];
}
