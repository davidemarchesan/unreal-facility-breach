// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "InventorySlot.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SUniformGridPanel.h"

class SInventoryOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryOverlay)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void OnToggle();
	
	void OnAddItem(FString ItemName);

private:
	TSharedPtr<SOverlay> MainOverlay;
	TSharedPtr<SUniformGridPanel> GridPanel;

	TArray<TArray<TSharedPtr<SInventorySlot>>> Inventory;

	const int32 Rows = 5;
	const int32 Cols = 5;
	
	int32 CurrentRow = 0;
	int32 CurrentCol = 0;
};
