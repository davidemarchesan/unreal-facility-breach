// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItems.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class FACILITYBREACH_API APickupItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	APickupItem();

	virtual void Tick(float DeltaTime) override;

	/* InteractableInterface */
	virtual bool IsInteractable() override { return true; };
	virtual FInteractionHint GetHint(APlayerController* PlayerController) override;

	virtual void OnInteract(APlayerController* PlayerController) override;
	virtual void OnFocus(APlayerController* PlayerController) override;
	virtual void OnFocusLost(APlayerController* PlayerController) override;
	/* END InteractableInterface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly);
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(Category = "Item", EditAnywhere);
	FDataTableRowHandle ItemTableRow;

private:
	FItemTableRow* Item;
};
