// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "AbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FACILITYBREACH_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Try to perform dash */
	void Dash();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<AFirstPersonCharacter> CharacterOwner;

	UPROPERTY(Category="Dash", EditAnywhere, BlueprintReadWrite, meta=(ForceUnits="s"))
	float DashCooldown;

	UPROPERTY(Category="Dash", EditAnywhere, BlueprintReadWrite)
	int32 DashCharges;

private:

	TObjectPtr<UFirstPersonMovementComponent> CachedMovementComponent;
	TObjectPtr<UFirstPersonMovementComponent> GetMovementComponent();

	void InitializeAbilities();

	void InitializeDash();
	
	float DashCurrentCooldown;

	UPROPERTY(Category="Dash", VisibleAnywhere)
	int32 DashCurrentCharges = 0;
	
	bool CanDash() const;

	void AddDashCharge();
	
	void ConsumeDashCharge();

	void ChargeDashUp(float deltaTime);

	bool bChargeDashUp = false;
	
};
