// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "FacilityBreach/Actors/PickupItems/PickupItems.h"
#include "FacilityBreach/PlayerControllers/FirstPersonController.h"
#include "FacilityBreach/PostProcess/Stencils/Stencils.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetRenderCustomDepth(false);
	}

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetupAttachment(RootComponent);
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("SphereComponent"));
	if (BoxComponent)
	{
		BoxComponent->SetupAttachment(RootComponent);
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPlayerBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnPlayerEndOverlap);
	}
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	RequiredItem = RequiredItemTableRow.GetRow<FItemTableRow>("Item look up");

	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}

	GameObjectivesSubsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>();
}

bool ADoor::HasRequiredItems(AFirstPersonController* Controller)
{
	if (RequiredItemTableRow.IsNull())
	{
		return true;
	}

	if (Controller == nullptr)
	{
		return true;
	}

	if (RequiredItem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ADoor::HasRequiredItems() Item look up failed"));
		return true;
	}

	return Controller->HasItemInInventory(RequiredItem->Name);
}

void ADoor::SetDoorState(EDoorState NewState)
{
	DoorState = NewState;
}

void ADoor::TryOpenDoor()
{
	if (DoorState != EDoorState::DOOR_Closed)
	{
		return;
	}

	SetDoorState(EDoorState::DOOR_Opening);
}

void ADoor::TryCloseDoor()
{
	if (DoorState == EDoorState::DOOR_Closed || DoorState == EDoorState::DOOR_Closing)
	{
		return;
	}
	SetDoorState(EDoorState::DOOR_Closing);
}

void ADoor::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
}

void ADoor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TryCloseDoor();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractionHint ADoor::GetHint(APlayerController* PlayerController)
{
	if (bInteractable == false)
	{
		return FInteractionHint();
	}

	if (DoorState != EDoorState::DOOR_Closed)
	{
		return FInteractionHint();
	}

	if (AFirstPersonController* Controller = Cast<AFirstPersonController>(PlayerController))
	{
		if (HasRequiredItems(Controller))
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

void ADoor::OnInteract(APlayerController* PlayerController)
{
	if (bInteractable == false)
	{
		return;
	}

	if (DoorState != EDoorState::DOOR_Closed)
	{
		return;
	}

	if (AFirstPersonController* Controller = Cast<AFirstPersonController>(PlayerController))
	{
		if (HasRequiredItems(Controller))
		{
			TryOpenDoor();
		}
	}
}

void ADoor::OnFocus(APlayerController* PlayerController)
{
	if (DoorState == EDoorState::DOOR_Closed && SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetCustomDepthStencilValue(static_cast<int32>(EStencilType::STENCIL_Outline));
		SkeletalMeshComponent->SetRenderCustomDepth(true);
	}
}

void ADoor::OnFocusLost(APlayerController* PlayerController)
{
	if (SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}
}

void ADoor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}

void ADoor::OnOpenCompleted()
{
	SetDoorState(EDoorState::DOOR_Open);

	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->Emit(this, GameObjectivesSubsystem->Tag_Action_DoorOpen);
	}
}

void ADoor::OnCloseCompleted()
{
	SetDoorState(EDoorState::DOOR_Closed);

	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->Emit(this, GameObjectivesSubsystem->Tag_Action_DoorClose);
	}
}
