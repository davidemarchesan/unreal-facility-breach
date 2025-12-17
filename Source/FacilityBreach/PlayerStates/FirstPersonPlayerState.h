// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Inventory/InventoryComponent.h"
#include "GameFramework/PlayerState.h"
#include "FirstPersonPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AFirstPersonPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AFirstPersonPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(Category = "Inventory", VisibleAnywhere)
	TObjectPtr<UInventoryComponent> Inventory;
	
};
