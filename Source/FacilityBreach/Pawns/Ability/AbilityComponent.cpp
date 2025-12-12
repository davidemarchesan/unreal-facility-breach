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
	if (AbilitiesDataTable != nullptr)
	{
		AbilitiesDataTable->ForeachRow<FAbilityTableRow>("Abilities Look Up",
		                                                 [this](const FName& Key, const FAbilityTableRow& Value)
		                                                 {
			                                                 const FAbilityState State = FAbilityState(
				                                                 Value.MaxCharges, Value.Cooldown,
				                                                 Value.bIsRechargeable);
			                                                 AbilityStates.Add(Value.Type, State);
		                                                 });
	}
}

void UAbilityComponent::AddChargeToAbility(EAbilityType AbilityType, int32 ChargesToAdd)
{
	if (FAbilityState* State = AbilityStates.Find(AbilityType))
	{
		State->CurrentCharges = FMath::Min(State->CurrentCharges + ChargesToAdd, State->MaxCharges);

		if (State->CurrentCharges < State->MaxCharges)
		{
			State->CurrentCooldown = State->Cooldown;
		}
	}
}

void UAbilityComponent::ConsumeAbilityCharge(EAbilityType AbilityType, int32 ChargesToConsume)
{
	if (FAbilityState* State = AbilityStates.Find(AbilityType))
	{
		State->CurrentCharges = FMath::Max(State->CurrentCharges - ChargesToConsume, 0);

		if (State->bIsRechargeable == true && State->CurrentCooldown <= 0.f)
		{
			// Starting cooldown
			State->CurrentCooldown = State->Cooldown;
		}
	}
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (TPair<EAbilityType, FAbilityState>& Ability : AbilityStates)
	{
		if (Ability.Value.bIsRechargeable == false) continue; // Cannot recharge this ability
		
		if (Ability.Value.CurrentCooldown > 0.f)
		{
			Ability.Value.CurrentCooldown = FMath::Max(Ability.Value.CurrentCooldown - DeltaTime, 0.f);
			if (Ability.Value.CurrentCooldown <= 0.f)
			{
				AddChargeToAbility(Ability.Key, 1);
			}
		}
	}
}

void UAbilityComponent::Dash()
{
	if (FAbilityState* State = AbilityStates.Find(EAbilityType::ABILITY_Dash))
	{
		if (State->HasCharges() == false)
		{
			return;
		}

		ConsumeAbilityCharge(EAbilityType::ABILITY_Dash, 1);
		if (UFirstPersonMovementComponent* MovementComponent = GetMovementComponent())
		{
			MovementComponent->DoDash();
		}
	}
}
