// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "FacilityBreach/Actors/PickupItems/PickupItems.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPlayerBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnPlayerEndOverlap);
	}
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

bool ADoor::HasRequiredItems(AFirstPersonCharacter* Character)
{
	if (RequiredItemTableRow.IsNull())
	{
		return true;
	}

	if (Character == nullptr)
	{
		return true;
	}

	RequiredItem = RequiredItemTableRow.GetRow<FItemTableRow>("Item look up");
	if (RequiredItem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ADoor::HasRequiredItems() Item look up failed"));
		return true;
	}

	return Character->HasItemInInventory(RequiredItem->Name);
}

void ADoor::SetDoorState(EDoorState NewState)
{
	DoorState = NewState;
	if (NewState != EDoorState::DOOR_Closed)
	{
		AnimationTime = 0.f;
	}
}

void ADoor::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	// SetDoorState(EDoorState::DOOR_Opening);
}

void ADoor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (DoorState == EDoorState::DOOR_Open)
	{
		SetDoorState(EDoorState::DOOR_Closing);
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorState == EDoorState::DOOR_Opening || DoorState == EDoorState::DOOR_Closing)
	{
		AnimationTime += DeltaTime;

		float Time = FMath::Clamp(AnimationTime / AnimationDuration, 0.f, 1.f);
		float AdjustedSlidingVelocity = Time < 0.5f
			                                ? FMath::InterpCircularOut(0.f, DoorSlidingVelocity, Time * 2)
			                                : FMath::InterpCircularIn(DoorSlidingVelocity, 0.f, (Time * 2) - 1.f);

		float Direction = DoorState == EDoorState::DOOR_Opening ? 1.f : -1.f;
		MeshComponent->AddLocalOffset(FVector(AdjustedSlidingVelocity * DeltaTime * Direction, 0.f, 0.f));

		if (AnimationTime >= AnimationDuration)
		{
			SetDoorState(DoorState == EDoorState::DOOR_Opening ? EDoorState::DOOR_Open : EDoorState::DOOR_Closed);
		}
	}
}

FInteractionHint ADoor::GetHint(APawn* PawnInstigator)
{
	if (bInteractable == false)
	{
		return FInteractionHint();
	}

	if (DoorState != EDoorState::DOOR_Closed)
	{
		return FInteractionHint();
	}

	if (AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(PawnInstigator))
	{
		if (HasRequiredItems(Character))
		{
			return FInteractionHint(FText::FromString("Open Door"));
		}
		else
		{
			if (RequiredItem)
			{
				return FInteractionHint(
					FText::FromString(FString::Printf(TEXT("%s required"), *RequiredItem->Name)),
					false
				);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADoor::GetHint() Required Item is null"));
			}
		}
	}

	return FInteractionHint();
}

void ADoor::OnInteract(APawn* PawnInstigator)
{
	if (bInteractable == false)
	{
		return;
	}

	if (DoorState != EDoorState::DOOR_Closed)
	{
		return;
	}

	if (AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(PawnInstigator))
	{
		if (HasRequiredItems(Character))
		{
			SetDoorState(EDoorState::DOOR_Opening);
		}
	}
}
