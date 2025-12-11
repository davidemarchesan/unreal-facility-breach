// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonMovementComponent.generated.h"

enum class ECustomMovementMode : uint8
{
	MOVE_Dashing,
};


/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UFirstPersonMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:

	// Custom physics
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void PhysDashing(float deltaTime, int32 Iterations);

public:

	// Dash
	UPROPERTY(Category="Character Movement: Dashing", EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dash Velocity", ClampMin="0", UIMin="0"))
	float DashVelocity;

	UPROPERTY(Category="Character Movement: Dashing", EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dash Duration", ClampMin="0", UIMin="0", ForceUnits="s"))
	float DashDuration;

private:

	float DashRemainingTime = 0.f;

	float DashVelocityScalar = 1000.f;

	bool bStartDash = false;

public:
	
	void DoDash();

	virtual bool IsDashing();
	
};
