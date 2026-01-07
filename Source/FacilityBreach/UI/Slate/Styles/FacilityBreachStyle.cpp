// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachStyle.h"

#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FFacilityBreachStyle::StyleInstance = nullptr;

struct FStyleIconData
{
	FName Name;
	FString Source;
	FVector2D Size;
};

void FFacilityBreachStyle::Initialize()
{
	if (StyleInstance.IsValid())
	{
		return;
	}

	StyleInstance = MakeShareable(new FSlateStyleSet("FacilityBreachStyle"));

	InitializeIcons();
	InitializeBrushes();

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
}

void FFacilityBreachStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}

const ISlateStyle& FFacilityBreachStyle::Get()
{
	return *StyleInstance;
}

void FFacilityBreachStyle::InitializeIcons()
{
	const TArray<FStyleIconData> StyleIcons = {
		FStyleIconData(
			"Ability.Dash.Icon",
			"/Game/UI/Assets/Icons/Abilities/Icon_Dash",
			FVector2D(128.f, 128.f)),
		FStyleIconData(
			"Ability.Invisibility.Icon",
			"/Game/UI/Assets/Icons/Abilities/Icon_Invisibility",
			FVector2D(128.f, 128.f)),
		FStyleIconData(
			"Ability.Scan.Icon",
			"/Game/UI/Assets/Icons/Abilities/Icon_Radar",
			FVector2D(128.f, 128.f)),

		FStyleIconData(
			"General.Awareness.Icon",
			"/Game/UI/Assets/Icons/General/Icon_Awareness",
			FVector2D(128.f, 128.f)),
	};

	for (const FStyleIconData& StyleIcon : StyleIcons)
	{
		StyleInstance->Set(StyleIcon.Name,
		                   new FSlateImageBrush(LoadObject<UTexture2D>(nullptr, *StyleIcon.Source), StyleIcon.Size));
	}
}

void FFacilityBreachStyle::InitializeBrushes()
{
	StyleInstance->Set("Brush.Ability.Slot.Enabled", new FSlateRoundedBoxBrush(FLinearColor::White, 5.f));
	StyleInstance->Set("Brush.Ability.Slot.Disabled", new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("999999")), 5.f));
	
	StyleInstance->Set("Brush.Interaction.Hint", new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.6), 5.f));
	StyleInstance->Set("Brush.Interaction.Key", new FSlateRoundedBoxBrush(FLinearColor::White, 4.f));
	
	StyleInstance->Set("Brush.Goal.ProgressBar.Background", new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.3), 4.f, FLinearColor(1.f, 1.f, 1.f, 0.7), 2.f));
	StyleInstance->Set("Brush.Goal.ProgressBar.InProgress", new FSlateRoundedBoxBrush(FLinearColor::Yellow, 4.f, FLinearColor::Transparent, 2.f));
	StyleInstance->Set("Brush.Goal.ProgressBar.Completed", new FSlateRoundedBoxBrush(FLinearColor::White, 4.f, FLinearColor::Transparent, 2.f));

	StyleInstance->Set("Brush.Objective.Banner.New", new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.6), 5.f));
	StyleInstance->Set("Brush.Objective.Banner.Completed", new FSlateRoundedBoxBrush(FLinearColor::White, 5.f));

	StyleInstance->Set("Brush.Inventory.Background", new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("0A1E14FF")), 5.f));
	StyleInstance->Set("Brush.Inventory.Slot.Background", new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("1A3A28FF")), 5.f, FLinearColor::Transparent, 1.f));
	StyleInstance->Set("Brush.Inventory.Slot.Background.Active", new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("1A3A28FF")), 5.f, FLinearColor::White, 1.f));
	
}
