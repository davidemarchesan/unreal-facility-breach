// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FFacilityBreachStyle
{
public:
	static void Initialize();
	static void Shutdown();
	
	static const ISlateStyle& Get();

private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;

	static void InitializeIcons();
	static void InitializeBrushes();
};
