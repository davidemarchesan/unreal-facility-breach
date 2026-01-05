// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AFirstPersonCharacter>(GetOwner());

	WorldScanSubsystem = GetWorld()->GetSubsystem<UWorldScanSubsystem>();

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
			                                                 // Creating state
			                                                 const FAbilityState State = FAbilityState(
				                                                 Value.MaxCharges, Value.Cooldown,
				                                                 Value.bIsRechargeable);
			                                                 AbilityStates.Add(Value.Type, State);

			                                                 // Creating sound effects map
			                                                 if (Value.Sound != nullptr)
			                                                 {
				                                                 AbilitySoundEffects.Add(Value.Type, Value.Sound);
			                                                 }
		                                                 });
	}
}

void UAbilityComponent::AddChargeToAbility(EAbilityType AbilityType, int32 ChargesToAdd)
{
	if (FAbilityState* State = AbilityStates.Find(AbilityType))
	{
		State->CurrentCharges = FMath::Min(State->CurrentCharges + ChargesToAdd, State->MaxCharges);
		OnAbilityChargesChange.Broadcast(AbilityType, State->CurrentCharges);

		if (State->CurrentCharges < State->MaxCharges)
		{
			StartAbilityCooldown(AbilityType);
		}
	}
}

void UAbilityComponent::ConsumeAbilityCharge(EAbilityType AbilityType, int32 ChargesToConsume)
{
	if (FAbilityState* State = AbilityStates.Find(AbilityType))
	{
		State->CurrentCharges = FMath::Max(State->CurrentCharges - ChargesToConsume, 0);
		OnAbilityChargesChange.Broadcast(AbilityType, State->CurrentCharges);

		if (State->bIsRechargeable == true && State->CurrentCooldown <= 0.f)
		{
			StartAbilityCooldown(AbilityType);
		}
	}
}

void UAbilityComponent::StartAbilityCooldown(EAbilityType AbilityType)
{
	if (FAbilityState* State = AbilityStates.Find(AbilityType))
	{
		State->CurrentCooldown = State->Cooldown;
		OnAbilityCooldownStart.Broadcast(AbilityType, State->Cooldown);
	}
}

void UAbilityComponent::PlayAbilitySound(EAbilityType AbilityType)
{
	if (UAudioComponent* Audio = CharacterOwner->GetAudioComponent())
	{
		if (TObjectPtr<USoundBase>* SoundPtr = AbilitySoundEffects.Find(AbilityType))
		{
			if (USoundBase* Sound = SoundPtr->Get())
			{
				Audio->SetSound(Sound);
				Audio->Play();
			}
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
				OnAbilityCooldownEnd.Broadcast(Ability.Key);
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
			PlayAbilitySound(EAbilityType::ABILITY_Dash);
		}
	}
}

void UAbilityComponent::Scan()
{
	if (FAbilityState* State = AbilityStates.Find(EAbilityType::ABILITY_Scan))
	{
		if (State->HasCharges() == false)
		{
			return;
		}

		ConsumeAbilityCharge(EAbilityType::ABILITY_Scan, 1);
		if (WorldScanSubsystem && CharacterOwner)
		{
			WorldScanSubsystem->StartScan(CharacterOwner->GetActorLocation());
			PlayAbilitySound(EAbilityType::ABILITY_Scan);
		}
	}
}
