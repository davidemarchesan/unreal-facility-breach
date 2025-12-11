// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonMovementComponent.h"

void UFirstPersonMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	// Only custom mode right now
	PhysDashing(deltaTime, Iterations);
}

void UFirstPersonMovementComponent::PhysDashing(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	if (bStartDash == false)
	{
		DashRemainingTime = DashDuration;
		bStartDash = true;
	}
	
	FVector AdjustedVelocity = Velocity * deltaTime;
	FHitResult Hit(1.f);
	
	FVector TargetVelocity = AdjustedVelocity;
	TargetVelocity.Normalize();
	TargetVelocity = TargetVelocity * MaxWalkSpeed * deltaTime;

	float t = DashRemainingTime / DashDuration;

	AdjustedVelocity = FMath::InterpEaseIn(TargetVelocity, AdjustedVelocity, t, 2.f);
	
	SafeMoveUpdatedComponent(AdjustedVelocity, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.Time < 1.f)
	{
		// We hit something, dash is over
		bStartDash = false;
		
		SetMovementMode(MOVE_Walking);
		return;
	}
	
	DashRemainingTime -= deltaTime;
	if (DashRemainingTime <= 0.f)
	{
		// Dash is over
		bStartDash = false;

		// Keep walking at max speed
		// and let walking mode handle it
		AdjustedVelocity.Normalize(); // Direction of the dash
		Velocity = AdjustedVelocity * MaxWalkSpeed;
		
		SetMovementMode(MOVE_Walking);
		return;
	}
}

void UFirstPersonMovementComponent::DoDash()
{

	// Get current movement (not facing) direction
	FVector VelocityDirection = Velocity;
	VelocityDirection.Z = 0.0f; // No jump
	
	if (VelocityDirection.IsNearlyZero())
	{
		// Dash forward if idle
		VelocityDirection = UpdatedComponent->GetForwardVector();
	}
	
	VelocityDirection.Normalize();
	
	// Dash velocity
	Velocity = VelocityDirection * DashVelocity * DashVelocityScalar;

	SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::MOVE_Dashing));
	
}

bool UFirstPersonMovementComponent::IsDashing()
{
	return MovementMode == MOVE_Custom
		&& CustomMovementMode == static_cast<uint8>(ECustomMovementMode::MOVE_Dashing)
		&& UpdatedComponent;
}
