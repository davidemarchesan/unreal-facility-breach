// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonMovementComponent.h"

void UFirstPersonMovementComponent::DoDash()
{

	if (IsFalling())
	{
		UE_LOG(LogTemp, Warning, TEXT("isfalling, no dash"));
		return;
	}

	// Get current direction
	FVector VelocityDirection = Velocity;
	VelocityDirection.Z = 0.0f; // No jump
	
	if (VelocityDirection.IsNearlyZero())
	{
		VelocityDirection = UpdatedComponent->GetForwardVector();
	}
	
	VelocityDirection.Normalize();

	 UE_LOG(LogTemp, Warning, TEXT("Velocity Direction %f %f %f"), VelocityDirection.X, VelocityDirection.Y, VelocityDirection.Z);

	// Dash velocity
	Velocity = VelocityDirection * DashVelocity;
	
}
