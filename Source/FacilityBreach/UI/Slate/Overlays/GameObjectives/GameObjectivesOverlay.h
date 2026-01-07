// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObjectiveBanner.h"
#include "FacilityBreach/Actors/GameObjectives/GameObjectives.h"
#include "Widgets/SCompoundWidget.h"

class SGameObjectivesOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameObjectivesOverlay) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void OnGameObjectiveNew(FGameObjectiveState CurrentObjectiveState);
	void OnGameObjectiveUpdate(FGameObjectiveState CurrentObjectiveState);
	void OnGameObjectiveCompleted(FGameObjectiveState CurrentObjectiveState);

private:

	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<SGameObjectiveBanner> GameObjectiveBanner;
	TSharedPtr<SVerticalBox> GoalsVerticalBox;

	void UpdateBanners(FGameObjectiveState CurrentObjectiveState);
	void UpdateGoals(FGameObjectiveState CurrentObjectiveState);

	void ShowOverlay();
	void HideOverlay();
	
	void ClearBanners();
	void ClearGoals();
	
};
