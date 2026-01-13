// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachStyle.h"

#include "Engine/Font.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FFacilityBreachStyle::StyleInstance = nullptr;

struct FStyleFontData
{
	FString Name;
	FString Source;
};

struct FStyleIconData
{
	FName Name;
	FString Source;
	FVector2D Size;
};

struct FStyleButtonData
{
	FString Name;
	FSlateBrush Normal;
	FSlateBrush Hovered;
	FSlateBrush Pressed;
	FSlateColor NormalForeground;
	FSlateColor HoveredForeground;
	FSlateColor PressedForeground;
};

void FFacilityBreachStyle::Initialize()
{
	if (StyleInstance.IsValid())
	{
		return;
	}

	StyleInstance = MakeShareable(new FSlateStyleSet("FacilityBreachStyle"));

	InitializeFonts();
	InitializeIcons();
	InitializeBrushes();
	InitializeButtons();

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

void FFacilityBreachStyle::InitializeFonts()
{
	const TArray<FStyleFontData> StyleFonts = {
		FStyleFontData(
			"Font.Light",
			"/Game/UI/Assets/Fonts/FiraCode/FiraCode-Light_Font.FiraCode-Light_Font"
		),
		FStyleFontData(
			"Font.Regular",
			"/Game/UI/Assets/Fonts/FiraCode/FiraCode-Regular_Font.FiraCode-Regular_Font"
		),
		FStyleFontData(
			"Font.Medium",
			"/Game/UI/Assets/Fonts/FiraCode/FiraCode-Medium_Font.FiraCode-Medium_Font"
		),
		FStyleFontData(
			"Font.SemiBold",
			"/Game/UI/Assets/Fonts/FiraCode/FiraCode-SemiBold_Font.FiraCode-SemiBold_Font"
		),
		FStyleFontData(
			"Font.Bold",
			"/Game/UI/Assets/Fonts/FiraCode/FiraCode-Bold_Font.FiraCode-Bold_Font"
		),
	};

	const TMap<FString, float> FontSizes = {
		{"help", 10.f},
		{"p", 14.f},
		{"h1", 24.f},
		{"h2", 22.f},
		{"h3", 20.f},
		{"h4", 18.f},
	};

	for (const FStyleFontData& StyleFont : StyleFonts)
	{
		for (const TPair<FString, float>& FontSize : FontSizes)
		{
			FString StyleFontName = StyleFont.Name + "." + FontSize.Key;
			// Example: FB.Font.Regular.sm or FB.Font.Regular.md

			StyleInstance->Set(*StyleFontName, FSlateFontInfo(
				                   LoadObject<UFont>(
					                   nullptr, *StyleFont.Source),
				                   FontSize.Value
			                   )
			);
		}
	}
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
	StyleInstance->Set("Brush.Ability.Slot.Disabled",
	                   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("999999")), 5.f));

	StyleInstance->Set("Brush.Interaction.Hint", new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.6), 5.f));
	StyleInstance->Set("Brush.Interaction.Key", new FSlateRoundedBoxBrush(FLinearColor::White, 4.f));

	StyleInstance->Set("Brush.Goal.ProgressBar.Background",
	                   new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.3), 4.f,
	                                             FLinearColor(1.f, 1.f, 1.f, 0.7), 2.f));
	StyleInstance->Set("Brush.Goal.ProgressBar.InProgress",
	                   new FSlateRoundedBoxBrush(FLinearColor::Yellow, 4.f, FLinearColor::Transparent, 2.f));
	StyleInstance->Set("Brush.Goal.ProgressBar.Completed",
	                   new FSlateRoundedBoxBrush(FLinearColor::White, 4.f, FLinearColor::Transparent, 2.f));

	StyleInstance->Set("Brush.Objective.Banner.New", new FSlateRoundedBoxBrush(FLinearColor(0.f, 0.f, 0.f, 0.6), 5.f));
	StyleInstance->Set("Brush.Objective.Banner.Completed", new FSlateRoundedBoxBrush(FLinearColor::White, 5.f));

	StyleInstance->Set("Brush.Inventory.Background",
	                   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("0A1E14FF")), 5.f));
	StyleInstance->Set("Brush.Inventory.Slot.Background",
	                   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("1A3A28FF")), 5.f,
	                                             FLinearColor::Transparent, 1.f));
	StyleInstance->Set("Brush.Inventory.Slot.Background.Active",
	                   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("1A3A28FF")), 5.f, FLinearColor::White,
	                                             1.f));

	StyleInstance->Set("Brush.EndGame.Background",
	                   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("0A1E14D9")), 0.f));

	StyleInstance->Set("Brush.Loading.Background",
					   new FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("0A1E14FF")), 0.f));
}

void FFacilityBreachStyle::InitializeButtons()
{
	const TArray<FStyleButtonData> StyleButtons = {
		FStyleButtonData(
			"Button.Yellow",
			FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("#ceab00")), 5.f),
			FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("#ffd935")), 5.f),
			FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("#ffd935")), 5.f),
			FLinearColor::Black,
			FLinearColor::Black,
			FLinearColor::Black
		),
		FStyleButtonData(
			"Button.Outline.Yellow",
			FSlateRoundedBoxBrush(FLinearColor::Transparent, 5.f, FLinearColor(FColor::FromHex("#ffd935")), 4.f),
			FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("#ffd935")), 5.f, FLinearColor(FColor::FromHex("#ffd935")), 4.f),
			FSlateRoundedBoxBrush(FLinearColor(FColor::FromHex("#ffd935")), 5.f, FLinearColor(FColor::FromHex("#ffd935")), 4.f),
			FLinearColor(FColor::FromHex("#ffd935")),
			FLinearColor::Black,
			FLinearColor::Black
		),
	};

	const TMap<FString, FMargin> ButtonSizes = {
		{"md", FMargin(12.f, 4.f)}
	};

	for (const FStyleButtonData& StyleButton : StyleButtons)
	{
		for (const TPair<FString, FMargin>& ButtonSize : ButtonSizes)
		{
			const FButtonStyle Style = FButtonStyle()
			                           .SetNormal(StyleButton.Normal)
			                           .SetHovered(StyleButton.Hovered)
			                           .SetPressed(StyleButton.Pressed)
			                           .SetNormalForeground(StyleButton.NormalForeground)
			                           .SetHoveredForeground(StyleButton.HoveredForeground)
			                           .SetPressedForeground(StyleButton.PressedForeground)
			                           .SetNormalPadding(ButtonSize.Value);

			FString StyleButtonName = StyleButton.Name + "." + ButtonSize.Key;
			StyleInstance->Set(*StyleButtonName, Style);
		}
	}
}
