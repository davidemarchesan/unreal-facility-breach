// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Pawns/Ability/Abilities.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/Interactables/InteractablesOverlay.h"
#include "GameFramework/HUD.h"
#include "FacilityBreachHUD.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AFacilityBreachHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

private:

	TObjectPtr<AFirstPersonCharacter> CharacterOwner;
	
	/** Subscriptions to delegates */
	void InitializeDelegatesSub();

	void InitializeDelegatesAbilities();
	UFUNCTION() void OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds);
	UFUNCTION() void OnAbilityCooldownEnd(EAbilityType AbilityType);
	UFUNCTION() void OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges);

	void InitializeDelegatesInteractables();
	UFUNCTION() void OnInteractableFocus(FText Hint);
	UFUNCTION() void OnInteractableFocusEnd();
	
	/** Overlays */
	void InitializeOverlays();
	void InitializeOverlayCrosshair();
	void InitializeOverlayAbilities();
	void InitializeOverlayInteractables();

	TSharedPtr<SAbilitiesOverlay> AbilitiesOverlay;
	TSharedPtr<SInteractablesOverlay> InteractablesOverlay;
	
};
