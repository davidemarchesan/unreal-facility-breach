// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities.h"
#include "Components/ActorComponent.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Subsystems/World/WorldScanSubsystem.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCooldownStart, EAbilityType, AbilityType, float, Seconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCooldownEnd, EAbilityType, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChargesChange, EAbilityType, AbilityType, int32, Charges);

USTRUCT(BlueprintType)
struct FAbilityState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 MaxCharges = 1;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentCharges = 0;

	UPROPERTY(VisibleAnywhere)
	float Cooldown = 10.f;

	UPROPERTY(VisibleAnywhere)
	float CurrentCooldown = 0.f;

	UPROPERTY(VisibleAnywhere)
	bool bIsRechargeable = true;

	FAbilityState() = default;

	FAbilityState(int32 InMaxCharges, float InCooldown, bool bInIsRechargeable)
		: MaxCharges(InMaxCharges)
		  , CurrentCharges(InMaxCharges)
		  , Cooldown(InCooldown)
		  , bIsRechargeable(bInIsRechargeable)
	{
	}

	bool IsOnCooldown() const { return CurrentCooldown > 0.f; }
	bool HasCharges() const { return CurrentCharges > 0; }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FACILITYBREACH_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TObjectPtr<UDataTable> AbilitiesDataTable;

	TMap<EAbilityType, FAbilityState> GetAbilityStates() const { return AbilityStates; };

	/** Delegates **/
	FOnAbilityCooldownStart OnAbilityCooldownStart;
	FOnAbilityCooldownEnd OnAbilityCooldownEnd;
	FOnAbilityChargesChange OnAbilityChargesChange;

	/** Try to perform dash */
	void Dash();

	/** Try to perform scan */
	void Scan();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<AFirstPersonCharacter> CharacterOwner;

	UPROPERTY(VisibleAnywhere)
	TMap<EAbilityType, FAbilityState> AbilityStates;

	UPROPERTY(VisibleAnywhere)
	TMap<EAbilityType, TObjectPtr<USoundBase>> AbilitySoundEffects;

private:
	TObjectPtr<UFirstPersonMovementComponent> CachedMovementComponent;
	TObjectPtr<UFirstPersonMovementComponent> GetMovementComponent();

	void InitializeAbilities();
	
	void AddChargeToAbility(EAbilityType AbilityType, int32 ChargesToAdd = 1);

	void ConsumeAbilityCharge(EAbilityType AbilityType, int32 ChargesToConsume = 1);

	void StartAbilityCooldown(EAbilityType AbilityType);

	void PlayAbilitySound(EAbilityType AbilityType);

	/** Subsystems */
	TObjectPtr<UWorldScanSubsystem> WorldScanSubsystem;
	UGameObjectivesSubsystem* GameObjectivesSubsystem;
	
};
