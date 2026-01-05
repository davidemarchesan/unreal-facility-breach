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
#include "FacilityBreach/UI/Widgets/AlertFeedbackWidget.h"
#include "EnemyGuardCharacter.generated.h"

UCLASS()
class FACILITYBREACH_API AEnemyGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetDetected();
	void SetUndetected();

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

	UFUNCTION() void OnEnterPatrol();
	UFUNCTION() void OnEnterSuspicious();
	UFUNCTION() void OnEnterChase();

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnSuspicious;

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnChase;

	UPROPERTY(Category="Audio", EditAnywhere)
	TObjectPtr<USoundBase> SoundOnDetected;

private:

	TObjectPtr<AFirstPersonCharacter> Player = nullptr;

	TObjectPtr<AEnemyGuardAIController> AIController;

	TObjectPtr<UAlertFeedbackWidget> AlertFeedbackWidget;
	
	bool IsPlayerInVision();
	bool bPlayerInVision = false;

	FTimerHandle DetectedTimerHandle;

};
