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
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
