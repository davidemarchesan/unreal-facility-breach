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
	virtual void Interact() override;
	/* END InteractableInterface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly);
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(Category = "Item", EditDefaultsOnly);
	FDataTableRowHandle ItemTableRow;

private:

	FItemTableRow* Item;

};
