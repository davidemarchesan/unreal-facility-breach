// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachHUD.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "FacilityBreach/Pawns/Ability/AbilityComponent.h"
#include "FacilityBreach/PlayerControllers/FirstPersonController.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"

#include "Widgets/SOverlay.h"

void AFacilityBreachHUD::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AFirstPersonCharacter>(GetOwningPawn());

	InitializeDelegatesSub();

	InitializeOverlays();
}

void AFacilityBreachHUD::InitializeDelegatesSub()
{
	InitializeDelegatesAbilities();
	InitializeDelegatesInteractables();
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
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityCooldownStart(AbilityType, Seconds);
	}
}

void AFacilityBreachHUD::OnAbilityCooldownEnd(EAbilityType AbilityType)
{
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityCooldownEnd(AbilityType);
	}
}

void AFacilityBreachHUD::OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges)
{
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityChargesChange(AbilityType, Charges);
	}
}

void AFacilityBreachHUD::InitializeDelegatesInteractables()
{
	AFirstPersonController* Controller = Cast<AFirstPersonController>(GetOwningPlayerController());
	if (Controller)
	{
		Controller->OnShowInteractionHint.AddDynamic(this, &AFacilityBreachHUD::OnShowInteractionHint);
		Controller->OnHideInteractionHint.AddDynamic(this, &AFacilityBreachHUD::OnHideInteractionHint);
	}
}

void AFacilityBreachHUD::OnShowInteractionHint(FInteractionHint Hint)
{
	if (InteractablesOverlay)
	{
		InteractablesOverlay->OnShowInteractionHint(Hint);
	}
}

void AFacilityBreachHUD::OnHideInteractionHint()
{
	if (InteractablesOverlay)
	{
		InteractablesOverlay->OnHideInteractionHint();
	}
}

void AFacilityBreachHUD::InitializeOverlays()
{
	if (GEngine && GEngine->GameViewport)
	{
		InitializeOverlayCrosshair();
		InitializeOverlayAbilities();
		InitializeOverlayInteractables();
		InitializeOverlayGameObjectives();
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
	AbilitiesOverlay = SNew(SAbilitiesOverlay);

	if (AbilitiesOverlay && CharacterOwner)
	{
		if (UAbilityComponent* AbilityComponent = CharacterOwner->GetAbilityComponent())
		{
			AbilitiesOverlay->InitializeAbilities(AbilityComponent->AbilitiesDataTable);
		}
		GEngine->GameViewport->AddViewportWidgetContent(AbilitiesOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayInteractables()
{
	InteractablesOverlay = SNew(SInteractablesOverlay);

	if (InteractablesOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(InteractablesOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayGameObjectives()
{
	GameObjectivesOverlay = SNew(SGameObjectivesOverlay);

	if (GameObjectivesOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(GameObjectivesOverlay.ToSharedRef());
	}
}
