#pragma once

#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/SCompoundWidget.h"

class SGameObjectiveBanner : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameObjectiveBanner)
		{
		}
		SLATE_ARGUMENT(EVisibility, Visibility)
		SLATE_ARGUMENT(bool, Completed)
		SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void Update(bool bInCompleted, const FText& InSubtitle);

private:

	TSharedPtr<SBox> MainBox;
	TSharedPtr<SBorder> MainBorder;
	TSharedPtr<STextBlock> TitleTextBlock;
	TSharedPtr<STextBlock> SubtitleTextBlock;

	const FText Title_New = FText::FromString("New objective");
	const FText Title_Completed = FText::FromString("Objective completed");

	const FSlateBrush* Brush_New = FFacilityBreachStyle::Get().GetBrush("Brush.Objective.Banner.New");
	const FSlateBrush* Brush_Completed = FFacilityBreachStyle::Get().GetBrush("Brush.Objective.Banner.Completed");

	const float FadeOutDelay_New = 6.f;
	const float FadeOutDelay_Completed = 4.f;

	float ConstructTime = 0.f;

	/** Fade out animation */
	float FadeOutDuration = 1.f;
	float FadeOutStartTime = 0.f;

	void StartFadingOut(float Delay = 1.f);
	bool bIsFading = false;
	bool bFadedOut = false;
	
};
