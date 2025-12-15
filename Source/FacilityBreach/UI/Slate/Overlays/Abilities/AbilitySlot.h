// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SAbilitySlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAbilitySlot)
		{}
		SLATE_ARGUMENT(int32, Charges)
		SLATE_ARGUMENT(FName, Icon)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:

	void OnAbilityCooldownStart(float Seconds);
	void OnAbilityCooldownEnd();
	void OnAbilityChargesChange(int32 Charges);

private:

	TSharedPtr<STextBlock> CooldownTextBlock;
	TSharedPtr<STextBlock> ChargesTextBlock;
	TSharedPtr<SBox> ChargingShroudBox;
	TSharedPtr<SBorder> IconBorder;

	const float AbilitySlotSize = 70.f;
	const float BorderRadius = 5.f;

	/** Cooldown */
	bool bCooldownStarted = false;		// Wether the cooldown is running
	double CooldownStartTime = 0.f; 	// When cooldown started
	float CooldownTotalSeconds = 0.f;	// Cooldown total seconds
	float Cooldown = 0.f;				// Current cooldown
	int32 CooldownShown = 0;			// Cooldown shown on UI

	void UpdateCooldown(float InCooldown);
	
};
