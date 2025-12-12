// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FacilityBreach/Pawns/Movement/FirstPersonMovementComponent.h"
#include "FirstPersonCharacter.generated.h"

class UAbilityComponent;
class UCameraComponent;

UCLASS()
class FACILITYBREACH_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

private:

	/** Component responsible for abilities' (like dash) settings, like cooldown, charges etc. */
	UPROPERTY(Category="Character", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityComponent;

	/** Custom camera component */
	UPROPERTY(Category="Character", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE UFirstPersonMovementComponent* GetCharacterMovement() const { return CastChecked<UFirstPersonMovementComponent>(GetMovementComponent()); }

	void Dash();
};
