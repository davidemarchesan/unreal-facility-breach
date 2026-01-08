// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonCharacter.h"
#include "Components/SphereComponent.h"
#include "FacilityBreach/Actors/WayPoints/WayPoint.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/UI/Widgets/AlertFeedbackWidget.h"
#include "EnemyGuardCharacter.generated.h"

UCLASS()
class FACILITYBREACH_API AEnemyGuardCharacter : public ACharacter, public IInteractableInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetDetected();
	void SetUndetected();

	bool IsDeactivated() const { return bDeactivated; }

	/* InteractableInterface */
	virtual bool IsInteractable() override { return true; };
	virtual FInteractionHint GetHint(APlayerController* PlayerController) override;

	virtual void OnInteract(APlayerController* PlayerController) override;
	virtual void OnFocus(APlayerController* PlayerController) override;
	virtual void OnFocusLost(APlayerController* PlayerController) override;
	/* END InteractableInterface */

	/* IGameplayTagAssetInterface */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/* END IGameplayTagAssetInterface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAudioComponent> AudioFeedbackComponent;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWidgetComponent> AlertFeedbackWidgetComponent;

	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Category="Patrol", EditInstanceOnly)
	bool bStartPatrolOnPlay = false;
	
	UPROPERTY(Category="Patrol", EditInstanceOnly)
	TArray<TObjectPtr<AWayPoint>> WayPoints;

	UPROPERTY(Category="Patrol", EditAnywhere)
	bool bLoopWayPoints = true;

	UPROPERTY(Category="Patrol", EditAnywhere)
	float VisionRange = 1000.f;
	
	UPROPERTY(Category="Patrol", EditAnywhere, meta=(Units="degrees"))
	float VisionAngle;

	UPROPERTY(Category="Patrol", EditAnywhere)
	float StandardSpeed = 300.f;

	UPROPERTY(Category="Patrol", EditAnywhere)
	float ChaseSpeed = 500.f;

	UFUNCTION() void OnEnterPatrol();
	UFUNCTION() void OnEnterSuspicious();
	UFUNCTION() void OnEnterChase();

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnSuspicious;

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnChase;

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnDetected;

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnDeactivated;

	/** Gameplay Tags */
	UPROPERTY(Category="Gameplay Tags", EditAnywhere)
	FGameplayTagContainer GameplayTags;

private:

	TObjectPtr<AFirstPersonCharacter> Player = nullptr;

	TObjectPtr<AEnemyGuardAIController> AIController;

	TObjectPtr<UAlertFeedbackWidget> AlertFeedbackWidget;
	
	bool IsPlayerInVision();
	bool IsPlayerBehind();
	bool bPlayerInVision = false;

	FTimerHandle DetectedTimerHandle;

	void SetPatrolSpeed();
	void SetChaseSpeed();
	void SetSpeed(float InSpeed);

	bool bDeactivated = false;
	void Deactivate();

	UGameObjectivesSubsystem* GameObjectivesSubsystem;

};
