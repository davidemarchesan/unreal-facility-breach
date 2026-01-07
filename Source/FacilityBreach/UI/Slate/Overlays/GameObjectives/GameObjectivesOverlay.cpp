#include "GameObjectivesOverlay.h"

#include "GameObjectiveBanner.h"
#include "GameObjectiveGoal.h"

void SGameObjectivesOverlay::Construct(const FArguments& InArgs)
{
	const float OverlayPaddingHorizontal = 60.f;
	const float OverlayPaddingVertical = 60.f;

	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)
		.Visibility(EVisibility::Collapsed)

		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.Padding(OverlayPaddingHorizontal, OverlayPaddingVertical)
		[
			SAssignNew(GameObjectiveBanner, SGameObjectiveBanner)
			.Visibility(EVisibility::Collapsed)
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding(OverlayPaddingHorizontal, OverlayPaddingVertical)
		[
			SAssignNew(GoalsVerticalBox, SVerticalBox)
		]
	];
}

void SGameObjectivesOverlay::OnGameObjectiveNew(FGameObjectiveState CurrentObjectiveState)
{
	UE_LOG(LogTemp, Warning, TEXT("SGameObjectivesOverlay::OnGameObjectiveNew"));

	if (CurrentObjectiveState.bActive == false)
	{
		HideOverlay();
		return;
	}

	UpdateBanners(CurrentObjectiveState);
	UpdateGoals(CurrentObjectiveState);
	ShowOverlay();
}

void SGameObjectivesOverlay::OnGameObjectiveUpdate(FGameObjectiveState CurrentObjectiveState)
{
	UpdateGoals(CurrentObjectiveState);
}

void SGameObjectivesOverlay::OnGameObjectiveCompleted(FGameObjectiveState CurrentObjectiveState)
{
	UpdateBanners(CurrentObjectiveState);
	UpdateGoals(CurrentObjectiveState);
}

void SGameObjectivesOverlay::UpdateBanners(FGameObjectiveState CurrentObjectiveState)
{
	if (GameObjectiveBanner)
	{
		GameObjectiveBanner->Update(CurrentObjectiveState.bCompleted, CurrentObjectiveState.Title);
	}
	
}

void SGameObjectivesOverlay::UpdateGoals(FGameObjectiveState CurrentObjectiveState)
{
	if (GoalsVerticalBox == nullptr)
	{
		return;
	}

	if (CurrentObjectiveState.Goals.Num() > 0)
	{
		GoalsVerticalBox->ClearChildren();

		for (FGameObjectiveGoalState Goal : CurrentObjectiveState.Goals)
		{
			GoalsVerticalBox->AddSlot()
			                .Padding(0.f, 5.f)
			[
				SNew(SGameObjectiveGoal)
				.Completed(Goal.bCompleted)
				.CurrentCount(Goal.CurrentCount)
				.Count(Goal.Count)
				.Title(Goal.Title)
			];
		}
	}
}

void SGameObjectivesOverlay::ShowOverlay()
{
	if (MainOverlay == nullptr)
	{
		return;
	}

	MainOverlay->SetVisibility(EVisibility::Visible);
}

void SGameObjectivesOverlay::HideOverlay()
{
	if (MainOverlay == nullptr)
	{
		return;
	}

	MainOverlay->SetVisibility(EVisibility::Collapsed);
}

void SGameObjectivesOverlay::ClearBanners()
{
	
}

void SGameObjectivesOverlay::ClearGoals()
{
	if (GoalsVerticalBox == nullptr)
	{
		return;
	}
	GoalsVerticalBox->ClearChildren();
}
