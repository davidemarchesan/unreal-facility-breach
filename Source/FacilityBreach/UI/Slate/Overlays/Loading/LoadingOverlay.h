#pragma once

#include "Widgets/SCompoundWidget.h"

class SLoadingOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingOverlay)
		{
		}
		SLATE_EVENT(FSimpleDelegate, OnFadeOut)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void Show();
	void FadeOut();
	

private:
	TSharedPtr<SOverlay> MainOverlay;

	float FadeOutDuration = 1.f;
	float FadeOutStartTime = 0.f;

	bool bIsFading = false;
	bool bFadedOut = false;

	FSimpleDelegate OnFadeOut;
};
