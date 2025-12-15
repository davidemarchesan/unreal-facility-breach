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
		SLATE_ARGUMENT(float, Cooldown)
		SLATE_ARGUMENT(FName, Icon)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:

	void OnAbilityCooldownStart(float Seconds);
	void OnAbilityCooldownEnd();
	void OnAbilityChargesChange(int32 Charges);

private:

	TSharedPtr<STextBlock> CooldownTextBlock;
	TSharedPtr<STextBlock> ChargesTextBlock;
	
};
