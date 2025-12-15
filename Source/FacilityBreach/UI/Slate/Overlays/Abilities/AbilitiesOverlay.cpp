// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesOverlay.h"

#include "AbilitySlot.h"


void SAbilitiesOverlay::Construct(const FArguments& InArgs)
{
	ChildSlot
	[

		SNew(SOverlay)

		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(40.f)
		[
			SAssignNew(AbilitiesHorizontalBox, SHorizontalBox)
		]

	];
}

void SAbilitiesOverlay::InitializeAbilities(TObjectPtr<UDataTable> AbilitiesDataTable)
{
	if (AbilitiesDataTable != nullptr)
	{
		AbilitiesDataTable->ForeachRow<FAbilityTableRow>("Abilities Look Up",
		                                                 [this](const FName& Key, const FAbilityTableRow& Value)
		                                                 {
			                                                 TSharedPtr<SAbilitySlot> AbilitySlot = SNew(SAbilitySlot)
				                                                 .Charges(Value.MaxCharges)
				                                                 .Icon(Value.Icon);

			                                                 if (AbilitySlot && AbilitiesHorizontalBox)
			                                                 {
				                                                 AbilitySlots.Add(Value.Type, AbilitySlot);


				                                                 AbilitiesHorizontalBox->AddSlot()
					                                                 .Padding(10.f, 0.f)
					                                                 [
						                                                 AbilitySlot.ToSharedRef()
					                                                 ];
			                                                 }
		                                                 });
	}
}

void SAbilitiesOverlay::OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds)
{
	if (TSharedPtr<SAbilitySlot>* AbilitySlot = AbilitySlots.Find(AbilityType))
	{
		AbilitySlot->ToSharedRef()->OnAbilityCooldownStart(Seconds);
	}
}

void SAbilitiesOverlay::OnAbilityCooldownEnd(EAbilityType AbilityType)
{
	if (TSharedPtr<SAbilitySlot>* AbilitySlot = AbilitySlots.Find(AbilityType))
	{
		AbilitySlot->ToSharedRef()->OnAbilityCooldownEnd();
	}
}

void SAbilitiesOverlay::OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges)
{
	if (TSharedPtr<SAbilitySlot>* AbilitySlot = AbilitySlots.Find(AbilityType))
	{
		AbilitySlot->ToSharedRef()->OnAbilityChargesChange(Charges);
	}
}
