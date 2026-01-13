#include "TutorialOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void STutorialOverlay::Construct(const FArguments& InArgs)
{

	const FText Title = InArgs._Title;
	const FText Description = InArgs._Description;
	
	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		.Padding(100.f)
		[
			SNew(SBox)
			.WidthOverride(800.f)
			[
				SNew(SBorder)
				.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Inventory.Background"))
				[

					SNew(SBox)
					.Padding(15.f)
					[

						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(TitleTextBlock, STextBlock)
							.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.SemiBold.h4"))
							.ColorAndOpacity(FLinearColor::Yellow)
							.Text(FText::GetEmpty())
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBox)
							[
								SAssignNew(DescriptionTextBlock, STextBlock)
								.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Regular.p"))
								.ColorAndOpacity(FLinearColor::White)
								.AutoWrapText(true)
								.Text(Description)
							]
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.f, 10.f, 0.f, 0.f)
						[
							SNew(SBox)
							[
								SNew(STextBlock)
								.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Bold.help"))
								.ColorAndOpacity(FLinearColor::White)
								.Text(FText::FromString("Press ESC to continue"))
							]
						]
					]

				]
			]
		]
	];
}

void STutorialOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
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
		const float Opacity = FMath::InterpEaseOut(1.f, 0.f, Alpha, 2.f);

		if (MainOverlay.IsValid())
		{
			if (FMath::IsNearlyZero(Opacity, 0.02f))
			{
				MainOverlay->SetVisibility(EVisibility::Collapsed);
				bIsFading = false;
				bFadedOut = true;
			}
			else
			{
				MainOverlay->SetRenderOpacity(Opacity);
			}
		}
	}
}

void STutorialOverlay::OnShow(FText InTitle, FText InDescription)
{
	if (MainOverlay.IsValid() && TitleTextBlock.IsValid() && DescriptionTextBlock.IsValid())
	{
		TitleTextBlock->SetText(FText::Format(FText::FromString("Tutorial - {0}"), InTitle));
		DescriptionTextBlock->SetText(InDescription);

		MainOverlay->SetRenderOpacity(1.f);
		MainOverlay->SetVisibility(EVisibility::Visible);

		FadeOutStartTime = FSlateApplication::Get().GetCurrentTime() + FadeOutDelay;
		bIsFading = true;
		bFadedOut = false;
	}
}

void STutorialOverlay::OnHide()
{
	if (MainOverlay.IsValid())
	{
		MainOverlay->SetVisibility(EVisibility::Collapsed);
		bFadedOut = true;
	}
}
