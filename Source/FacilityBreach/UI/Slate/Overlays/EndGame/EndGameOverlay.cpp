#include "EndGameOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SEndGameOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo Font = FFacilityBreachStyle::Get().GetFontStyle("Font.Bold.h1");
	Font.Size = 34.f;

	FOnClicked OnRespawn = InArgs._OnRespawn;

	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[

			SNew(SBorder)
			.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.EndGame.Background"))
			[

				SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[

					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.f, 0.f, 0.f, 15.f)
					[
						SNew(STextBlock)
						.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Bold.h1"))
						.Text(FText::FromString("You're dead"))
						.ColorAndOpacity(FColor::Yellow)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[

						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.f, 0.f, 0.f, 5.f)
						[
							SNew(SButton)
							.ButtonStyle(&FFacilityBreachStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Yellow.md"))
							.OnClicked(OnRespawn)
							[
								SNew(STextBlock)
								.Text(FText::FromString("Respawn"))
								.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.h4"))
								.AutoWrapText(true)
								.TransformPolicy(ETextTransformPolicy::ToUpper)
								.Justification(ETextJustify::Center)
							]
						]
						
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SButton)
							.ButtonStyle(&FFacilityBreachStyle::Get().GetWidgetStyle<FButtonStyle>("Button.Outline.Yellow.md"))
							[
								SNew(STextBlock)
								.Text(FText::FromString("Exit"))
								.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.h4"))
								.AutoWrapText(true)
								.TransformPolicy(ETextTransformPolicy::ToUpper)
								.Justification(ETextJustify::Center)
							]
						]

					]

				]

			]

		]
	];
}
