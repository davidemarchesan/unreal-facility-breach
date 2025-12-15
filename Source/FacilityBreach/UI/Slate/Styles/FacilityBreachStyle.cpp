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
