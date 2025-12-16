// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySlot.h"
#include "FacilityBreach/Pawns/Ability/Abilities.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SAbilitiesOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAbilitiesOverlay)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:

	void InitializeAbilities(TObjectPtr<UDataTable> AbilitiesDataTable);

	void OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds);
	void OnAbilityCooldownEnd(EAbilityType AbilityType);
	void OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges);

private:

	TMap<EAbilityType, TSharedPtr<SAbilitySlot>> AbilitySlots;

	TSharedPtr<SHorizontalBox> AbilitiesHorizontalBox;
	
};
