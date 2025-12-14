// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonController.generated.h"

class UInputMappingContext;
class UFirstPersonInputConfig;
struct FInputActionValue;

class AFirstPersonCharacter;

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AFirstPersonController : public APlayerController
{
	GENERATED_BODY()

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


	// Debug only
	void Debug();
	
};
