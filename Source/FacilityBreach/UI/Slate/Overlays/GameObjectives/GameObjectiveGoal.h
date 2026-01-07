// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Widgets/SCompoundWidget.h"

class SGameObjectiveGoal : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameObjectiveGoal) {}
		SLATE_ARGUMENT(bool, Completed)
		SLATE_ARGUMENT(int32, CurrentCount)
		SLATE_ARGUMENT(int32, Count)
		SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
};
