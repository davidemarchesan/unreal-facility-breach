#pragma once

#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Widgets/SCompoundWidget.h"

class SGameObjectivesOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameObjectivesOverlay) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void UpdateGameObjective(const FGameObjectiveState& CurrentObjectiveState);

	void ClearGameObjective();

private:

	TSharedPtr<SVerticalBox> GoalsVerticalBox;
	
};
