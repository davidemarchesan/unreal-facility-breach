#include "LoadingOverlay.h"

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void SLoadingOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(FFacilityBreachStyle::Get().GetBrush("Brush.Loading.Background"))
			[
				SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(40.f)
				[
					SNew(STextBlock)
					.Font(FFacilityBreachStyle::Get().GetFontStyle("Font.Bold.h4"))
					.Text(FText::FromString("Loading..."))
					.ColorAndOpacity(FColor::Yellow)
				]
			]
		]

	];
}

void SLoadingOverlay::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (bFadedOut == true)
	{
		return;
	}

	if (bIsFading == true)
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

void SLoadingOverlay::Show()
{
	if (MainOverlay.IsValid())
	{
		MainOverlay->SetRenderOpacity(1.f);
		MainOverlay->SetVisibility(EVisibility::Visible);
	}
}

void SLoadingOverlay::FadeOut()
{
	FadeOutStartTime = FSlateApplication::Get().GetCurrentTime();
	bIsFading = true;
	bFadedOut = false;
}
