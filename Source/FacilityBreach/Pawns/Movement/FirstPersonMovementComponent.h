// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UFirstPersonMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	// Dash
	UPROPERTY(Category="Character Movement: Dashing", EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dash Velocity", ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float DashVelocity;
	
	void DoDash();
	
};
