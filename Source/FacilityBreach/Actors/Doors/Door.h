// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "FacilityBreach/Actors/PickupItems/PickupItems.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/PlayerControllers/FirstPersonController.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	DOOR_Closed,
	DOOR_Opening,
	DOOR_Open,
	DOOR_Closing
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
	virtual FInteractionHint GetHint(APlayerController* PlayerController) override;

	virtual void OnInteract(APlayerController* PlayerController) override;
	virtual void OnFocus(APlayerController* PlayerController) override;
	virtual void OnFocusLost(APlayerController* PlayerController) override;
	/* END InteractableInterface */

	EDoorState GetDoorState() { return DoorState; }

	void OnOpenCompleted();
	void OnCloseCompleted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	/** Sliding animation */
	UPROPERTY(Category="Animation", EditAnywhere)
	TObjectPtr<UAnimMontage> AnimationMontage;
	
	UPROPERTY(Category="Animation", EditAnywhere)
	TObjectPtr<UAnimationAsset> AnimationOpen;

	UPROPERTY(Category="Animation", EditAnywhere)
	TObjectPtr<UAnimationAsset> AnimationClose;
	
	/** Interaction */
	UPROPERTY(Category="Interaction", EditAnywhere)
	bool bInteractable = false;

	UPROPERTY(Category="Interaction", EditAnywhere)
	FDataTableRowHandle RequiredItemTableRow;

	bool HasRequiredItems(AFirstPersonController* Controller);
	FItemTableRow* RequiredItem;

private:

	UPROPERTY(EditAnywhere)
	EDoorState DoorState = EDoorState::DOOR_Closed;
	
	void TryOpenDoor();
	void TryCloseDoor();
	void SetDoorState(EDoorState NewState);

	// FTimerHandle TryCloseTimerHandle;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
