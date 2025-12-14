// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
};
