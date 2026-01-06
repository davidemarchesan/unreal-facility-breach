#include "GameObjectivesOverlay.h"

#include "GameObjectiveGoal.h"

void SGameObjectivesOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding((160.f))
		// .Padding((40.f))
		[

			SAssignNew(GoalsVerticalBox, SVerticalBox)
			
		]
	];
}

void SGameObjectivesOverlay::UpdateGameObjective(const FGameObjectiveState& CurrentObjectiveState)
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
				.Goal(Goal)
			];
		}
	}
}

void SGameObjectivesOverlay::ClearGameObjective()
{
	if (GoalsVerticalBox == nullptr)
	{
		return;
	}

	GoalsVerticalBox->ClearChildren();
}
