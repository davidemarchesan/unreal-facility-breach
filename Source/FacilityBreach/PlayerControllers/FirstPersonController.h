// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "FacilityBreach/Actors/PickupItems/PickupItems.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Subsystems/LocalPlayer/LocalPlayerAudioSubsystem.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "FacilityBreach/Subsystems/World/TutorialSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonController.generated.h"

class UInputMappingContext;
class UFirstPersonInputConfig;
struct FInputActionValue;

class AFirstPersonCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowInteractionHint, FInteractionHint, Hint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideInteractionHint);

DECLARE_MULTICAST_DELEGATE(FOnInventoryToggle);

DECLARE_MULTICAST_DELEGATE(FOnControllerReady);

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AFirstPersonController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	/** Delegates */
	FOnShowInteractionHint OnShowInteractionHint;
	FOnHideInteractionHint OnHideInteractionHint;

	FOnInventoryToggle OnInventoryToggle;

	bool IsReady() const { return bReady;}
	FOnControllerReady OnControllerReady;

	virtual void EnableInput(class APlayerController* PlayerController) override;
	virtual void DisableInput(class APlayerController* PlayerController) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	// Input Mapping Contexts
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FirstPersonInputMappingContext;

	void InitializeMappingContexts();

	// Input Configs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UFirstPersonInputConfig> FirstPersonInputConfig;

	// Input Actions triggers
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Interact();

	void Jump();

	void Dash();

	void Scan();

	void ToggleInventory();

	void Back();

	// Character
	AFirstPersonCharacter* FirstPersonCharacter;
	UCameraComponent* FirstPersonCameraComponent;

	// Line trace
	float LineTraceRayLength = 250.f;
	void LineTrace();
	AActor* LineTraceHitActor = nullptr;

	void UnFocusInteractable(AActor* Actor);
	
	// Subsystems
	void LoadSubsystems();
	ULocalPlayerAudioSubsystem* AudioSubsystem;
	UGameObjectivesSubsystem* GameObjectivesSubsystem;
	UTutorialSubsystem* TutorialSubsystem;

	// Tutorial
	UFUNCTION() void OnTutorialShow(const FText& Title, const FText& Description);
	bool bShowingTutorial = false;

	// Debug only
	void Debug();

	UFUNCTION() void OnPlayerDeath();
	UFUNCTION()	void OnPlayerRespawn();

	bool bReady = false;
	UFUNCTION() void OnLevelReady();

public:

	/** Inventory */
	void AddItemToInventory(FString ItemName, int32 Quantity = 1);
	void AddItemToInventory(AActor* Actor, FItemTableRow* Item, int32 Quantity = 1);
	bool HasItemInInventory(FString ItemName, int32 QuantityRequired = 1);
	
};
