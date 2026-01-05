#pragma once

#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Widgets/SCompoundWidget.h"

class SGameObjectiveGoal : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameObjectiveGoal) {}
		SLATE_ARGUMENT(FGameObjectiveGoalState, Goal)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
};
