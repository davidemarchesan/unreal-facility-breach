// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	GameObjectivesSubsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>();
}

void UInventoryComponent::AddItem(FString ItemName, int32 Quantity)
{
	Items.Add(ItemName, Quantity);

	OnAddItem.Broadcast(ItemName);
}

void UInventoryComponent::AddItem(AActor* Actor, FItemTableRow* Item, int32 Quantity)
{
	Items.Add(Item->Name, Quantity);

	OnAddItem.Broadcast(Item->Name);
	
	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->Emit(Actor, GameObjectivesSubsystem->Tag_Action_Item_PickUp);
	}
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
