// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddItem, FString);

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FACILITYBREACH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	void AddItem(FString ItemName, int32 Quantity = 1);
	bool HasItem(FString ItemName, int32 QuantityRequired = 1);

	FOnAddItem OnAddItem;

private:

	UPROPERTY(VisibleAnywhere)
	TMap<FString, int32> Items = {};
	
};
