// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "FacilityBreach/Pawns/Movement/FirstPersonMovementComponent.h"
#include "Components/AudioComponent.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "FirstPersonCharacter.generated.h"

class UAbilityComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class FACILITYBREACH_API AFirstPersonCharacter : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE UFirstPersonMovementComponent* GetCharacterMovement() const { return CastChecked<UFirstPersonMovementComponent>(GetMovementComponent()); }

	FORCEINLINE UAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return FirstPersonCameraComponent; }
	
	FORCEINLINE UAudioComponent* GetAudioComponent() const { return AudioComponent; }

	virtual void Jump() override;
	
	void Dash();

	void Scan();

	FOnDeath OnDeath;

	void RespawnCharacter(FVector Location, FRotator Rotation);
	void RespawnCharacter();

	/* IGameplayTagAssetInterface */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/* END IGameplayTagAssetInterface */

protected:
	virtual void BeginPlay() override;

	UFUNCTION() void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Gameplay Tags */
	UPROPERTY(Category="Gameplay Tags", EditAnywhere)
	FGameplayTagContainer GameplayTags;
	
private:

	/** Component responsible for abilities' (like dash) settings, like cooldown, charges etc. */
	UPROPERTY(Category="Character", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityComponent;

	/** Custom camera component */
	UPROPERTY(Category="Character", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(Category="Character", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> AudioComponent;

	UGameObjectivesSubsystem* GameObjectivesSubsystem;

	bool bDead = false;
	void Die();

	FVector InitialLocation;
	FRotator InitialRotation;
	
};
