// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachHUD.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"
#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

#include "Widgets/SOverlay.h"

void AFacilityBreachHUD::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeOverlayCrosshair();

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SAbilitiesOverlay));
	}
}

void AFacilityBreachHUD::InitializeOverlayCrosshair()
{
	if (GEngine && GEngine->GameViewport)
	{
		const TSharedRef<SOverlay> Overlay = SNew(SOverlay);

		Overlay->AddSlot()
		       .HAlign(HAlign_Center)
		       .VAlign(VAlign_Center)
		[

			SNew(SBorder)
			.BorderImage(new FSlateRoundedBoxBrush(FLinearColor::White, 2.f, FLinearColor::Black, 1.f))
			[

				SNew(SBox)
				.HeightOverride(2.f)
				.WidthOverride(2.f)
				
			]
		
		];

		GEngine->GameViewport->AddViewportWidgetContent(Overlay);
	}
}
