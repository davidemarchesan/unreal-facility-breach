// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "FacilityBreach/Actors/PickupItems/PickupItems.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	DOOR_Closed,
	DOOR_Opening,
	DOOR_Open,
	DOOR_Closing,
};

UCLASS()
class FACILITYBREACH_API ADoor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	virtual void Tick(float DeltaTime) override;

	/* InteractableInterface */
	virtual bool IsInteractable() override { return bInteractable; };
	virtual FText GetHint(APawn* PawnInstigator) override;

	virtual void OnInteract(APawn* PawnInstigator) override;
	/* END InteractableInterface */

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	/** Sliding animation */
	UPROPERTY(Category="Animation", EditAnywhere, meta=(ForceUnits="s"))
	float AnimationDuration = 1.f;

	UPROPERTY(Category="Animation", EditAnywhere)
	float DoorSlidingVelocity = 1.f;

	/** Interaction */
	UPROPERTY(Category="Interaction", EditAnywhere)
	bool bInteractable = false;

	UPROPERTY(Category="Interaction", EditAnywhere)
	FDataTableRowHandle RequiredItemTableRow;

	bool HasRequiredItems(AFirstPersonCharacter* Character);
	FItemTableRow* RequiredItem;

private:

	UPROPERTY(EditAnywhere)
	EDoorState DoorState = EDoorState::DOOR_Closed;

	void SetDoorState(EDoorState NewState);

	float AnimationTime = 0.f;
	
	float CurrentDoorTranslationOffest = 0.f;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
