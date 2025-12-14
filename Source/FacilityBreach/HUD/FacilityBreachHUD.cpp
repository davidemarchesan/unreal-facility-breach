// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachHUD.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "FacilityBreach/Pawns/Ability/AbilityComponent.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"

#include "Widgets/SOverlay.h"

void AFacilityBreachHUD::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = CastChecked<AFirstPersonCharacter>(GetOwningPawn());

	InitializeDelegatesSub();
	
	InitializeOverlays();
	
}

void AFacilityBreachHUD::InitializeDelegatesSub()
{
	InitializeDelegatesAbilities();
}

void AFacilityBreachHUD::InitializeDelegatesAbilities()
{
	if (CharacterOwner == nullptr)
	{
		return;
	}

	UAbilityComponent* AbilityComponent = CharacterOwner->GetAbilityComponent();
	if (AbilityComponent == nullptr)
	{
		return;
	}
	
	AbilityComponent->OnAbilityCooldownStart.AddDynamic(this, &AFacilityBreachHUD::OnAbilityCooldownStart);
	AbilityComponent->OnAbilityCooldownEnd.AddDynamic(this, &AFacilityBreachHUD::OnAbilityCooldownEnd);
	AbilityComponent->OnAbilityChargesChange.AddDynamic(this, &AFacilityBreachHUD::OnAbilityChargesChange);
	
}

void AFacilityBreachHUD::OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD: an ability has started cooldown"));
}

void AFacilityBreachHUD::OnAbilityCooldownEnd(EAbilityType AbilityType)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD: an ability has ended cooldown"));
}

void AFacilityBreachHUD::OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges)
{
	UE_LOG(LogTemp, Warning, TEXT("HUD: an ability has new charges %d"), Charges);
}

void AFacilityBreachHUD::InitializeOverlays()
{
	if (GEngine && GEngine->GameViewport)
	{
		InitializeOverlayCrosshair();
		InitializeOverlayAbilities();
	}
}

void AFacilityBreachHUD::InitializeOverlayCrosshair()
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

void AFacilityBreachHUD::InitializeOverlayAbilities()
{
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SAbilitiesOverlay));
}
