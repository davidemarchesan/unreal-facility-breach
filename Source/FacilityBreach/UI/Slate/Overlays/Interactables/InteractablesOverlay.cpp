// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractablesOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SInteractablesOverlay::Construct(const FArguments& InArgs)
{
	FSlateFontInfo KeyTextFont = FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.p");
	FSlateFontInfo HintTextFont = FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.p");

	const float ContainerWidth = 400.f;
	const float ContainerPaddingLeft = 100.f;
	const float ContainerPaddingTop = 35.f;

	const float HintBoxPadding = 9.f;
	const float KeyBoxSize = 33.f;

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	ChildSlot
	[

		SAssignNew(RootOverlay, SOverlay)
		.Visibility(EVisibility::Visible)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[

			// Container at the center of the screen
			// translated to the right
			SNew(SBox)
			.WidthOverride(ContainerWidth)
			.Padding(ContainerPaddingLeft, ContainerPaddingTop, 0.f, 0.f)
			.RenderTransform(FSlateRenderTransform(FVector2D(ContainerWidth * 0.5f, 0.f)))
			[

				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[

					SNew(SBorder)
					.Padding(HintBoxPadding)
					.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Interaction.Hint"))
					[

						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
						[

							SAssignNew(InputBox, SBox)
							.Padding(0.f, 0.f, 10.f, 0.f)
							[
								SNew(SBox)
								.HeightOverride(KeyBoxSize)
								.WidthOverride(KeyBoxSize)
								[

									SNew(SBorder)
									.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Interaction.Key"))
									.Padding(0.f)
									[

										SNew(SOverlay)

										+ SOverlay::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										[
											SNew(STextBlock)
											.ColorAndOpacity(FLinearColor::Black)
											.Text(FText::FromString("E"))
											.Font(KeyTextFont)
											.Justification(ETextJustify::Center)
										]

									]
								]
							]


						]

						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)

						.AutoWidth()
						[

							SAssignNew(HintTextBlock, STextBlock)
							.ColorAndOpacity(FLinearColor::White)
							.Text(FText::FromString("Pick up item from overlay custom"))
							.Font(HintTextFont)
						]

					]
				]

			]

		]

	];
}

void SInteractablesOverlay::OnShowInteractionHint(FInteractionHint Hint)
{
	if (Hint.Text.IsEmpty())
	{
		OnHideInteractionHint();
	}

	if (HintTextBlock.IsValid())
	{
		HintTextBlock->SetText(Hint.Text);
	}
	if (InputBox.IsValid())
	{
		InputBox->SetVisibility(Hint.bShowInput ? EVisibility::Visible : EVisibility::Collapsed);
	}
	if (RootOverlay.IsValid())
	{
		RootOverlay->SetVisibility(EVisibility::Visible);
	}
}

void SInteractablesOverlay::OnHideInteractionHint()
{
	if (HintTextBlock.IsValid())
	{
		HintTextBlock->SetText(FText::GetEmpty());
	}
	if (RootOverlay.IsValid())
	{
		RootOverlay->SetVisibility(EVisibility::Hidden);
	}
}
