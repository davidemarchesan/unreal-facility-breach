// Fill out your copyright notice in the Description page of Project Settings.

#include "GameObjectiveGoal.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SGameObjectiveGoal::Construct(const FArguments& InArgs)
{
	const bool bCompleted = InArgs._Completed;
	const int32 CurrentCount = InArgs._CurrentCount;
	const int32 Count = InArgs._Count;
	const FText Title = InArgs._Title;

	FSlateFontInfo TitleFont = FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.p");
	TitleFont.OutlineSettings = FFontOutlineSettings(1, FLinearColor(0.f, 0.f, 0.f, 0.6));

	FSlateFontInfo ProgressBarFont = FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.help");

	const float GoalBoxWidth = 350.f;

	FText GoalText = bCompleted
		                 ? FText::FromString("Completed")
		                 : FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentCount, Count));

	const float ProgressBarWidth = CurrentCount > 0
		                               ? static_cast<float>(CurrentCount) / static_cast<float>(Count)
		                               : 0.f;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(GoalBoxWidth)
		[

			SNew(SVerticalBox)

			// Goal Title
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 0.f, 0.f, 1.f)
			[
				SNew(SBox)
				[

					SNew(STextBlock)
					.Font(TitleFont)
					.AutoWrapText(true)
					.ColorAndOpacity(FLinearColor::White)
					.Text(Title)
				]
			]

			// Goal Progress Bar
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				[

					SNew(SOverlay)

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SBox)
						.WidthOverride(GoalBoxWidth)
						[
							SNew(SBorder)
							.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Goal.ProgressBar.Background"))
						]
					]

					+ SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Fill)
					[
						SNew(SBox)
						.WidthOverride(GoalBoxWidth * ProgressBarWidth)
						[

							SNew(SBorder)
							.BorderImage(
								FFacilityBreachStyle::Get().GetBrush(bCompleted ? "Brush.Goal.ProgressBar.Completed" : "Brush.Goal.ProgressBar.InProgress"))

						]
					]

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SBox)
						.Padding(9.f, 3.f)
						[
							SNew(STextBlock)
							.Font(ProgressBarFont)
							.ColorAndOpacity(CurrentCount > 0 ? FLinearColor::Black : FLinearColor::White)
							.Text(GoalText)
						]
					]

				]
			]

		]
	];
}
