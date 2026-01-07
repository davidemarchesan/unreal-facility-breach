// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "FacilityBreach/Actors/Portals/Portal.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Subsystems/LocalPlayer/LocalPlayerAudioSubsystem.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonController.generated.h"

class UInputMappingContext;
class UFirstPersonInputConfig;
struct FInputActionValue;

class AFirstPersonCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowInteractionHint, FInteractionHint, Hint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideInteractionHint);

DECLARE_MULTICAST_DELEGATE(FOnInventoryToggle);

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

	// Character
	AFirstPersonCharacter* FirstPersonCharacter;
	UCameraComponent* FirstPersonCameraComponent;

	// Line trace
	float LineTraceRayLength = 250.f;
	void LineTrace();
	AActor* LineTraceHitActor = nullptr;
	
	// Subsystems
	void LoadSubsystems();
	ULocalPlayerAudioSubsystem* AudioSubsystem;
	UGameObjectivesSubsystem* GameObjectivesSubsystem;

	// Debug only
	void Debug();

public:

	/** Inventory */
	void AddItemToInventory(FString ItemName, int32 Quantity = 1);
	bool HasItemInInventory(FString ItemName, int32 QuantityRequired = 1);
	
};
