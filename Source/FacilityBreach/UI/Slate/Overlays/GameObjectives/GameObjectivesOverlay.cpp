#include "GameObjectivesOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SGameObjectivesOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	TitleFont.Size = 12.f;
	TitleFont.OutlineSettings = FFontOutlineSettings(1, FLinearColor(0.f, 0.f, 0.f, 0.6));

	FSlateFontInfo ProgressBarFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	ProgressBarFont.Size = 10.f;

	const float GoalBoxWidth = 350.f;

	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding((80.f))
		// .Padding((40.f))
		[

			SNew(SVerticalBox)

			// Goal active
			+ SVerticalBox::Slot()
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
							.Text(FText::FromString("Open the door"))
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
								.WidthOverride(200.f)
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
										.Text(FText::FromString("0/1"))
									]
								]
							]

						]
					]

				]

			]

		]
	];
}
