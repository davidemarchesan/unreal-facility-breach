// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySlot.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/Layout/SScaleBox.h"

void SAbilitySlot::Construct(const FArguments& InArgs)
{
	const int32 Charges = InArgs._Charges;
	const FName Icon = InArgs._Icon;

	FSlateFontInfo CooldownTextFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	CooldownTextFont.Size = 16.f;
	CooldownTextFont.OutlineSettings = FFontOutlineSettings(1.f, FLinearColor::Black);

	ChildSlot
	[
		SNew(SBox)
		.HeightOverride(AbilitySlotSize)
		.WidthOverride(AbilitySlotSize)
		[

			SNew(SOverlay)

			// Ability image
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[

				SAssignNew(IconBorder, SBorder)
				.BorderImage(
					new FSlateRoundedBoxBrush(FLinearColor(AbilitySlotEnabledColor), BorderRadius))
				.Padding(10.f)
				[
					SNew(SScaleBox)
					.Stretch(EStretch::ScaleToFit)
					.StretchDirection(EStretchDirection::DownOnly)
					[

						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Black)
						.Image(FFacilityBreachStyle::Get().GetBrush(Icon))
					]
				]
			]

			// Vertically charging shroud
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ChargingShroudBox, SBox)
				.WidthOverride(AbilitySlotSize)
				.HeightOverride(0.f)
				.Visibility(EVisibility::Hidden)
				[
					SNew(SBorder)
					.BorderImage(
						new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.3f), BorderRadius))
				]
			]

			// Ability disabled shroud
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.Visibility(EVisibility::Hidden)
				.BorderImage(new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.5f), BorderRadius))
			]

			// Charges
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(5.f)
			[
				SAssignNew(ChargesTextBlock, STextBlock)
				.Text(FText::AsNumber(Charges))
				.ColorAndOpacity(FLinearColor::Black)
			]

			// Cooldown
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(CooldownTextBlock, STextBlock)
				.Visibility(EVisibility::Hidden)
				.Text(FText::AsNumber(0))
				.Font(CooldownTextFont)
				.ColorAndOpacity(FLinearColor::White)
			]


		]
	];
}

void SAbilitySlot::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (bCooldownStarted == true)
	{
		double SecondsPassed = FMath::Abs(CooldownStartTime - InCurrentTime);
		UpdateCooldown(CooldownTotalSeconds - SecondsPassed);
	}
}

void SAbilitySlot::OnAbilityCooldownStart(float Seconds)
{
	if (CooldownTextBlock && ChargingShroudBox)
	{
		CooldownTotalSeconds = Seconds;
		CooldownStartTime = FSlateApplication::Get().GetCurrentTime();

		UpdateCooldown(Seconds);

		bCooldownStarted = true;

		CooldownTextBlock->SetVisibility(EVisibility::Visible);
		ChargingShroudBox->SetVisibility(EVisibility::Visible);
	}
}

void SAbilitySlot::OnAbilityCooldownEnd()
{
	bCooldownStarted = false;
	if (CooldownTextBlock && ChargingShroudBox)
	{
		CooldownTextBlock->SetVisibility(EVisibility::Hidden);
		ChargingShroudBox->SetVisibility(EVisibility::Hidden);
	}
}

void SAbilitySlot::OnAbilityChargesChange(int32 Charges)
{
	if (ChargesTextBlock)
	{
		ChargesTextBlock->SetText(FText::AsNumber(Charges));
	}

	if (IconBorder)
	{
		IconBorder->SetBorderImage(
			new FSlateRoundedBoxBrush(
				FLinearColor(Charges > 0 ? AbilitySlotEnabledColor : AbilitySlotDisabledColor),
				BorderRadius
			)
		);
	}
}

void SAbilitySlot::UpdateCooldown(float InCooldown)
{
	Cooldown = InCooldown;

	CooldownShown = FMath::CeilToInt(Cooldown);
	CooldownTextBlock->SetText(FText::AsNumber(CooldownShown));

	// BoxWidth is also the 100% height
	if (ChargingShroudBox)
	{
		float NewHeight = (AbilitySlotSize * Cooldown) / CooldownTotalSeconds;
		ChargingShroudBox->SetHeightOverride(NewHeight);
	}
}
