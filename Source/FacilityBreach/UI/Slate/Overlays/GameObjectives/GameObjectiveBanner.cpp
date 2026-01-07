// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObjectiveBanner.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SGameObjectiveBanner::Construct(const FArguments& InArgs)
{
	const EVisibility Visibility = InArgs._Visibility;
	const FText Title = InArgs._Title;

	FSlateFontInfo TitleFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	TitleFont.Size = 20.f;
	TitleFont.LetterSpacing = 1.f;

	FSlateFontInfo SubtitleFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	SubtitleFont.Size = 13.f;
	
	ChildSlot
	[
		SAssignNew(MainBox, SBox)
		.WidthOverride(400.f)
		.Padding(10.f)
		.Visibility(Visibility)
		[
			SAssignNew(MainBorder, SBorder)
			.BorderImage(Brush_New)
			[

				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				[
					SAssignNew(TitleTextBlock, STextBlock)
					.Font(TitleFont)
					.ColorAndOpacity(FLinearColor::White)
					.Justification(ETextJustify::Center)
					.TransformPolicy(ETextTransformPolicy::ToUpper)
					.Text(Title_New)
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				[
					SAssignNew(SubtitleTextBlock, STextBlock)
					.Font(SubtitleFont)
					.ColorAndOpacity(FLinearColor::White)
					.Justification(ETextJustify::Center)
					.Text(Title)
				]

			]
		]
	];
}

void SGameObjectiveBanner::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if (bFadedOut == true)
	{
		return;
	}
	
	if (bIsFading)
	{
		const float ElapsedTimeSinceFadingOut = InCurrentTime - FadeOutStartTime;
		if (ElapsedTimeSinceFadingOut < 0.f)
		{
			return;
		}

		const float Alpha = ElapsedTimeSinceFadingOut / FadeOutDuration;
		float Opacity = FMath::InterpEaseOut(1.f, 0.f, Alpha, 2.f);
		if (MainBox)
		{
			if (FMath::IsNearlyZero(Opacity, 0.02f))
			{
				MainBox->SetVisibility(EVisibility::Collapsed);
				bIsFading = false;
				bFadedOut = true;
			}
			else
			{
				MainBox->SetRenderOpacity(Opacity);
			}
		}
	}
}

void SGameObjectiveBanner::Update(bool bInCompleted, const FText& InSubtitle)
{
	if (MainBox == nullptr || MainBorder == nullptr || TitleTextBlock == nullptr || SubtitleTextBlock == nullptr)
	{
		return;
	}

	MainBorder->SetBorderImage(bInCompleted ? Brush_Completed : Brush_New);

	const FLinearColor TextColor = bInCompleted ? FLinearColor::Black : FLinearColor::White;

	TitleTextBlock->SetText(bInCompleted ? Title_Completed : Title_New);
	TitleTextBlock->SetColorAndOpacity(TextColor);

	SubtitleTextBlock->SetText(InSubtitle);
	SubtitleTextBlock->SetColorAndOpacity(TextColor);

	MainBox->SetRenderOpacity(1.f);
	MainBox->SetVisibility(EVisibility::Visible);

	StartFadingOut(bInCompleted ? FadeOutDelay_Completed : FadeOutDelay_New);
}

void SGameObjectiveBanner::StartFadingOut(float Delay)
{
	FadeOutStartTime = FSlateApplication::Get().GetCurrentTime() + Delay;
	bIsFading = true;
	bFadedOut = false;
}
