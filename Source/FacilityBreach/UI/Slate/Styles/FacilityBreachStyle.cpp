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

	StyleInstance->Set("Brush.Interaction.Key", new FSlateRoundedBoxBrush(FLinearColor::Transparent, 5.f, FLinearColor::White, 5.f));
	
}
