// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"
#include "Widgets/SCompoundWidget.h"

class SInventorySlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventorySlot)
	{
	}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Update(FText InText);

private:
	
	TSharedPtr<SBorder> MainBorder;
	TSharedPtr<STextBlock> TextBlock;

	const FSlateBrush* Brush_Standard = FFacilityBreachStyle::Get().GetBrush("Brush.Inventory.Slot.Background");
	const FSlateBrush* Brush_Active = FFacilityBreachStyle::Get().GetBrush("Brush.Inventory.Slot.Background.Active");
	
};
