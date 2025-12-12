// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "FacilityBreach/Pawns/FirstPersonCharacter.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AFirstPersonCharacter>(GetOwner());

	InitializeAbilities();
}

TObjectPtr<UFirstPersonMovementComponent> UAbilityComponent::GetMovementComponent()
{
	if (CachedMovementComponent == nullptr)
	{
		if (CharacterOwner)
		{
			CachedMovementComponent = CharacterOwner->GetCharacterMovement();
		}
	}

	return CachedMovementComponent;
}

void UAbilityComponent::InitializeAbilities()
{
	InitializeDash();
}

void UAbilityComponent::InitializeDash()
{
	DashCurrentCharges = DashCharges;
}

bool UAbilityComponent::CanDash() const
{
	if (UFirstPersonMovementComponent* MovementComponent = GetMovementComponent())
	{
		if (MovementComponent->IsDashing() || DashCurrentCharges <= 0)
		{
			return false;
		}

		return true;
	}

	return false;
}

void UAbilityComponent::AddDashCharge()
{
	DashCurrentCharges = FMath::Min(DashCurrentCharges + 1, DashCharges);

	if (bChargeDashUp == true)
	{
		if (DashCurrentCharges >= DashCharges)
		{
			// Stop charging dash
			bChargeDashUp = false;
		}
		else
		{
			// Keep charging dash
			DashCurrentCooldown = DashCooldown;
		}
	}
}

void UAbilityComponent::ConsumeDashCharge()
{
	DashCurrentCharges = FMath::Max(DashCurrentCharges - 1, 0);

	if (bChargeDashUp == false)
	{
		// Starting cooldown
		bChargeDashUp = true;
		DashCurrentCooldown = DashCooldown;
	}
}

void UAbilityComponent::ChargeDashUp(float deltaTime)
{
	DashCurrentCooldown -= deltaTime;

	if (DashCurrentCooldown <= 0.f)
	{
		AddDashCharge();
	}
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bChargeDashUp == true)
	{
		ChargeDashUp(DeltaTime);
	}
}

void UAbilityComponent::Dash()
{
	if (CanDash())
	{
		ConsumeDashCharge();
		if (UFirstPersonMovementComponent* MovementComponent = GetMovementComponent())
		{
			MovementComponent->DoDash();
		}
	}
}
