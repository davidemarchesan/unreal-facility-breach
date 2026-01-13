// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Widgets/SCompoundWidget.h"

class SEndGameOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEndGameOverlay) {}
		SLATE_ARGUMENT(bool, Dead);
		SLATE_EVENT(FOnClicked, OnRespawn)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedPtr<SOverlay> MainOverlay;
	
};
