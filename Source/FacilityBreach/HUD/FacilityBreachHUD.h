// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/GameModes/GameModeTest.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Pawns/Ability/Abilities.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/EndGame/EndGameOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/GameObjectives/GameObjectivesOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/Interactables/InteractablesOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/Inventory/InventoryOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/Tutorial/TutorialOverlay.h"
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

	UFUNCTION() void OnPlayerDeath();

private:

	TObjectPtr<AFirstPersonCharacter> CharacterOwner;

	AGameModeTest* GameMode;
	
	/** Subscriptions to delegates */
	void InitializeDelegatesSub();

	void InitializeDelegatesAbilities();
	UFUNCTION() void OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds);
	UFUNCTION() void OnAbilityCooldownEnd(EAbilityType AbilityType);
	UFUNCTION() void OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges);

	void InitializeDelegatesInteractables();
	UFUNCTION() void OnShowInteractionHint(FInteractionHint Hint);
	UFUNCTION() void OnHideInteractionHint();

	void InitializeDelegatesGameObjectives();
	UFUNCTION() void OnGameObjectiveNew(const FGameObjectiveState& CurrentObjectiveState);
	UFUNCTION() void OnGameObjectiveUpdate(const FGameObjectiveState& CurrentObjectiveState);
	UFUNCTION() void OnGameObjectiveCompleted(const FGameObjectiveState& CurrentObjectiveState);

	void InitializeDelegatesInventory();
	UFUNCTION() void OnInventoryToggle();
	UFUNCTION() void OnInventoryAddItem(FString ItemName);

	void InitializeDelegatesTutorial();
	UFUNCTION() void OnTutorialShow(const FText& Title, const FText& Description);
	UFUNCTION() void OnTutorialHide();

	void InitializeDelegatesEndGame();
	
	/** Overlays */
	void InitializeOverlays();
	void InitializeOverlayCrosshair();
	void InitializeOverlayAbilities();
	void InitializeOverlayInteractables();
	void InitializeOverlayGameObjectives();
	void InitializeOverlayInventory();
	void InitializeOverlayTutorial();
	void InitializeOverlayEndGame();

	TSharedPtr<SAbilitiesOverlay> AbilitiesOverlay;
	TSharedPtr<SInteractablesOverlay> InteractablesOverlay;
	TSharedPtr<SGameObjectivesOverlay> GameObjectivesOverlay;
	TSharedPtr<SInventoryOverlay> InventoryOverlay;
	TSharedPtr<STutorialOverlay> TutorialOverlay;
	TSharedPtr<SEndGameOverlay> EndGameOverlay;
	
};
