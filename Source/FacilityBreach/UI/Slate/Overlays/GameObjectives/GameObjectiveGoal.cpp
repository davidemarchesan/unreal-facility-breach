#include "GameObjectiveGoal.h"

#include <string>

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SGameObjectiveGoal::Construct(const FArguments& InArgs)
{

	const FGameObjectiveGoalState Goal = InArgs._Goal;
	
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	TitleFont.Size = 12.f;
	TitleFont.OutlineSettings = FFontOutlineSettings(1, FLinearColor(0.f, 0.f, 0.f, 0.6));

	FSlateFontInfo ProgressBarFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	ProgressBarFont.Size = 10.f;

	const float GoalBoxWidth = 350.f;

	FText GoalText = Goal.bCompleted ? FText::FromString("Completed") : FText::FromString(FString::Printf(TEXT("%d/%d"), Goal.CurrentCount, Goal.Count));

	const float ProgressBarWidth = Goal.CurrentCount > 0 ? static_cast<float>(Goal.CurrentCount) / static_cast<float>(Goal.Count) : 0.f;
	
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
					.Text(Goal.Title)
				]
			]

			// Goal Progress Bar
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				[

					SNew(SOverlay)

					// Actual progressbar
					+ SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Fill)
					[
						SNew(SBox)
						.WidthOverride(ProgressBarWidth * GoalBoxWidth)
						[
							SNew(SBorder)
							.BorderImage(
								FFacilityBreachStyle::Get().GetBrush("Brush.Goal.ProgressBar.InProgress"))
						]
					]

					// Background
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[

						SNew(SBorder)
						.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Goal.ProgressBar.Background"))
						[

							SNew(SBox)
							.Padding(9.f, 3.f)
							[

								SNew(STextBlock)
								.Font(ProgressBarFont)
								.ColorAndOpacity(FLinearColor::White)
								.Text(GoalText)
							]
						]
					]

				]
			]

		]
	];
}
