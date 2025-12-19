// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FacilityBreach/Input/Configs/FirstPersonInputConfig.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Pawns/FirstPersonPawn.h"
#include "FacilityBreach/PlayerStates/FirstPersonPlayerState.h"
#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void AFirstPersonController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	LineTrace();
}

void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();

	InitializeMappingContexts();

	FirstPersonCharacter = Cast<AFirstPersonCharacter>(GetPawn());
	if (FirstPersonCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FirstPersonController: No Character found!"));
	}

	if (FirstPersonCharacter)
	{
		FirstPersonCameraComponent = FirstPersonCharacter->GetCameraComponent();
	}
}

void AFirstPersonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (FirstPersonInputConfig)
		{
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_PrimaryAction, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::PrimaryAction);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_SecondaryAction, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::SecondaryAction);

			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Move, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Move);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Look, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Look);

			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Jump, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Jump);

			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Interact, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Interact);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Dash, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Dash);

			// Debug only
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Debug, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Debug);
		}
	}
}

void AFirstPersonController::InitializeMappingContexts()
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (FirstPersonInputMappingContext)
			{
				InputSystem->AddMappingContext(FirstPersonInputMappingContext, 1);
			}
		}
	}
}

void AFirstPersonController::PrimaryAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary action!"));

	if (PortalOne != nullptr)
	{
		PortalOne->Destroy();
	}

	const FVector Forward = FirstPersonCameraComponent->GetForwardVector().GetSafeNormal();

	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + (Forward * PortalLineTraceLength);

	FHitResult OutHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECC_Visibility
	);

	if (bHit)
	{
		if (PortalClass)
		{
			PortalOne = GetWorld()->SpawnActor<APortal>(PortalClass, OutHit.Location, OutHit.Normal.Rotation());
			if (PortalOne)
			{
				PortalOne->SetType(EPortalType::PORTAL_Blue);

				if (PortalTwo)
				{
					PortalOne->LinkPortal(PortalTwo);
					PortalTwo->LinkPortal(PortalOne);
				}
			}
		}
	}
}

void AFirstPersonController::SecondaryAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Secondary action!"));

	if (PortalTwo != nullptr)
	{
		PortalTwo->Destroy();
	}

	const FVector Forward = FirstPersonCameraComponent->GetForwardVector().GetSafeNormal();

	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + (Forward * PortalLineTraceLength);

	FHitResult OutHit;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECC_Visibility
	);

	if (bHit)
	{
		if (PortalClass)
		{
			PortalTwo = GetWorld()->SpawnActor<APortal>(PortalClass, OutHit.Location, OutHit.Normal.Rotation());
			if (PortalTwo)
			{
				PortalTwo->SetType(EPortalType::PORTAL_Red);

				if (PortalOne)
				{
					PortalOne->LinkPortal(PortalTwo);
					PortalTwo->LinkPortal(PortalOne);
				}
			}
		}
	}
}

void AFirstPersonController::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	if (FirstPersonCharacter != nullptr)
	{
		FirstPersonCharacter->AddMovementInput(FirstPersonCharacter->GetActorForwardVector(), InputVector.X);
		FirstPersonCharacter->AddMovementInput(FirstPersonCharacter->GetActorRightVector(), InputVector.Y);
	}
}

void AFirstPersonController::Look(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	if (FirstPersonCharacter != nullptr)
	{
		FirstPersonCharacter->AddControllerPitchInput(InputVector.Y);
		FirstPersonCharacter->AddControllerYawInput(InputVector.X);
	}
}

void AFirstPersonController::Interact()
{
	if (LineTraceHitActor == nullptr)
	{
		return;
	}

	if (LineTraceHitActor->IsActorBeingDestroyed())
	{
		return;
	}

	if (LineTraceHitActor->Implements<UInteractableInterface>())
	{
		TScriptInterface<IInteractableInterface> InteractableScriptInterface = TScriptInterface<
			IInteractableInterface>(LineTraceHitActor);
		if (InteractableScriptInterface && InteractableScriptInterface->IsInteractable() == true)
		{
			InteractableScriptInterface->OnInteract(this);
		}
	}
}

void AFirstPersonController::Jump()
{
	if (FirstPersonCharacter != nullptr)
	{
		FirstPersonCharacter->Jump();
	}
}

void AFirstPersonController::Dash()
{
	if (FirstPersonCharacter != nullptr)
	{
		FirstPersonCharacter->Dash();
	}
}

void AFirstPersonController::LineTrace()
{
	if (FirstPersonCameraComponent)
	{
		const FVector Forward = FirstPersonCameraComponent->GetForwardVector().GetSafeNormal();

		const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		const FVector End = Start + (Forward * LineTraceRayLength);

		FHitResult OutHit;

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			OutHit,
			Start,
			End,
			ECC_Visibility
		);

		if (bHit)
		{
			if (OutHit.GetActor())
			{
				if (OutHit.GetActor()->IsActorBeingDestroyed())
				{
					return;
				}

				if (LineTraceHitActor != nullptr && OutHit.GetActor() == LineTraceHitActor)
				{
					// Same actor
					return;
				}

				if (LineTraceHitActor == nullptr || OutHit.GetActor() != LineTraceHitActor)
				{
					// We got a new actor
					LineTraceHitActor = OutHit.GetActor();

					// Check if it is interactable
					if (LineTraceHitActor->Implements<UInteractableInterface>())
					{
						TScriptInterface<IInteractableInterface> InteractableScriptInterface = TScriptInterface<
							IInteractableInterface>(LineTraceHitActor);
						if (InteractableScriptInterface && InteractableScriptInterface->IsInteractable() == true)
						{
							FInteractionHint InteractionHint = InteractableScriptInterface->GetHint(this);
							OnShowInteractionHint.Broadcast(InteractionHint);

							return;
						}
					}
				}
			}
		}

		LineTraceHitActor = nullptr;
		OnHideInteractionHint.Broadcast();
	}
}

void AFirstPersonController::Debug()
{
	FFacilityBreachStyle::Shutdown();
	FFacilityBreachStyle::Initialize();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			2.f,
			FColor::Blue,
			TEXT("Style has been reset")
		);
	}
}

void AFirstPersonController::AddItemToInventory(FString ItemName, int32 Quantity)
{
	if (AFirstPersonPlayerState* State = GetPlayerState<AFirstPersonPlayerState>())
	{
		if (State->Inventory)
		{
			State->Inventory->AddItem(ItemName, Quantity);
		}
	}
}

bool AFirstPersonController::HasItemInInventory(FString ItemName, int32 QuantityRequired)
{
	if (AFirstPersonPlayerState* State = GetPlayerState<AFirstPersonPlayerState>())
	{
		if (State->Inventory)
		{
			return State->Inventory->HasItem(ItemName, QuantityRequired);
		}
	}
	return false;
}
