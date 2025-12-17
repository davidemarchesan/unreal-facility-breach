// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "PickupItems.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	
	
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	Item = ItemTableRow.GetRow<FItemTableRow>("Item look up");
	if (Item == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item from DT_Items table not found"));
	}
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText APickupItem::GetHint(APawn* PawnInstigator)
{
	if (Item)
	{
		return FText::FromString(FString::Printf(TEXT("Pick up %s"), *Item->Name));
	}
	
	return FText::GetEmpty();
}

void APickupItem::OnInteract(APawn* PawnInstigator)
{
	if (Item == nullptr)
	{
		return;
	}

	if (AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(PawnInstigator))
	{
		Character->AddItemToInventory(Item->Name);
		Destroy();
	}

	return;
}

