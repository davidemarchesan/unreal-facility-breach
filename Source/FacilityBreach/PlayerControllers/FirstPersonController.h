// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonController.generated.h"

class UInputMappingContext;
class UFirstPersonInputConfig;
struct FInputActionValue;

class AFirstPersonCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFocus, TScriptInterface<IInteractableInterface>, InteractableScriptInterface);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableFocusEnd);

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
	FOnInteractableFocus OnInteractableFocus;
	FOnInteractableFocusEnd OnInteractableFocusEnd;

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

	// Character
	AFirstPersonCharacter* FirstPersonCharacter;
	UCameraComponent* FirstPersonCameraComponent;

	// Line trace
	float LineTraceRayLength = 170.f;
	void LineTrace();
	AActor* LineTraceHitActor = nullptr;

	// Debug only
	void Debug();
	
};
