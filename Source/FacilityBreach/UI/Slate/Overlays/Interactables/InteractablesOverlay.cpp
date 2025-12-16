// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractablesOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SInteractablesOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo TextFont = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText").Font;
	TextFont.Size = 24.f;
	TextFont.OutlineSettings = FFontOutlineSettings(1.f, FLinearColor::Black);

	const float KeyBoxSize = 60.f;

	float ViewportHeight = 0.f;

	if (GEngine)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		ViewportHeight = ViewportSize.Y;
	}

	ChildSlot
	[

		SAssignNew(RootOverlay, SOverlay)
		.Visibility(EVisibility::Hidden)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[

			SNew(SBox)
			.HeightOverride(ViewportHeight * 0.5f)
			[
				SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[

						SNew(SBox)
						.HeightOverride(KeyBoxSize)
						.WidthOverride(KeyBoxSize)
						[

							SNew(SBorder)
							.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Interaction.Key"))
							.Padding(10.f)
							[

								SNew(STextBlock)
								.ColorAndOpacity(FLinearColor::White)
								.Text(FText::FromString("E"))
								.Font(TextFont)
								.Justification(ETextJustify::Center)
							]
						]

					]

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(10.f, 0.f, 0.f, 0.f)
					.AutoWidth()
					[

						SAssignNew(HintTextBlock, STextBlock)
						.ColorAndOpacity(FLinearColor::White)
						.Text(FText::FromString("Pick up item from overlay custom"))
						.Font(TextFont)
					]
				]
			]

		]

	];
}

void SInteractablesOverlay::OnInteractableFocus(FText Hint)
{
	if (HintTextBlock)
	{
		HintTextBlock->SetText(Hint);
	}
	if (RootOverlay)
	{
		RootOverlay->SetVisibility(EVisibility::Visible);
	}
}

void SInteractablesOverlay::OnInteractableFocusEnd()
{
	if (HintTextBlock)
	{
		HintTextBlock->SetText(FText::GetEmpty());
	}
	if (RootOverlay)
	{
		RootOverlay->SetVisibility(EVisibility::Hidden);
	}
}
