// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	DOOR_Idle,
	DOOR_Closing,
	DOOR_Opening
};

UCLASS()
class FACILITYBREACH_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(Category="Animation", EditAnywhere, meta=(ForceUnits="s"))
	float AnimationDuration = 1.f;

	UPROPERTY(Category="Animation", EditAnywhere)
	float DoorSlidingVelocity = 1.f;

private:

	UPROPERTY(VisibleAnywhere)
	EDoorState DoorState = EDoorState::DOOR_Idle;

	void SetDoorState(EDoorState NewState);

	float AnimationTime = 0.f;
	
	float CurrentDoorTranslationOffest = 0.f;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
