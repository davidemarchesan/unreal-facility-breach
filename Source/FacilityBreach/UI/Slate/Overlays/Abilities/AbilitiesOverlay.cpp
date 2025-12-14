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

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.Padding(10.f, 0.f)
			[
				SNew(SAbilitySlot)
			]

			+ SHorizontalBox::Slot()
			.Padding(10.f, 0.f)
			[
				SNew(SAbilitySlot)
			]

		]

	];
}
