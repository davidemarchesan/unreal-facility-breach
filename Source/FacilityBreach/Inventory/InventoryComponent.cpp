// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(FString ItemName, int32 Quantity)
{
	Items.Add(ItemName, Quantity);

	OnAddItem.Broadcast(ItemName);
}

bool UInventoryComponent::HasItem(FString ItemName, int32 QuantityRequired)
{
	if (int32* Quantity = Items.Find(ItemName))
	{
		if (*Quantity >= QuantityRequired)
		{
			return true;
		}

		return false;
	}
	return false;
}