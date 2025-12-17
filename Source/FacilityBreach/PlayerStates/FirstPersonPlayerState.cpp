// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPlayerState.h"

AFirstPersonPlayerState::AFirstPersonPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Inventory = CreateDefaultSubobject<UInventoryComponent>(FName("Inventory"));
}
