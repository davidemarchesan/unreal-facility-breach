// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FacilityBreach/GameModes/GameModeTest.h"
#include "FacilityBreach/Input/Configs/FirstPersonInputConfig.h"
#include "FacilityBreach/Interfaces/InteractableInterface.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Pawns/FirstPersonPawn.h"
#include "FacilityBreach/PlayerStates/FirstPersonPlayerState.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void AFirstPersonController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bReady == false)
	{
		bReady = true;
		OnControllerReady.Broadcast();
	}

	LineTrace();
}

void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();

	FirstPersonCharacter = Cast<AFirstPersonCharacter>(GetPawn());
	if (FirstPersonCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FirstPersonController: No Character found!"));
	}

	if (FirstPersonCharacter)
	{
		FirstPersonCameraComponent = FirstPersonCharacter->GetCameraComponent();
	}

	LoadSubsystems();

	if (AGameModeTest* GameMode = Cast<AGameModeTest>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnPlayerDeath.AddUObject(this, &AFirstPersonController::OnPlayerDeath);
		GameMode->OnLevelReady.AddUObject(this, &AFirstPersonController::OnLevelReady);
	}
}

void AFirstPersonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (FirstPersonInputConfig)
		{
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Move, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Move);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Look, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Look);

			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Jump, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Jump);

			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Interact, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Interact);

			// Abilities
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Dash, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Dash);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Scan, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Scan);

			// Others
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Tab, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::ToggleInventory);
			EnhancedInput->BindAction(FirstPersonInputConfig->IA_Back, ETriggerEvent::Triggered, this,
			                          &AFirstPersonController::Back);
			

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

void AFirstPersonController::Scan()
{
	if (FirstPersonCharacter != nullptr)
	{
		FirstPersonCharacter->Scan();
	}
}

void AFirstPersonController::ToggleInventory()
{
	OnInventoryToggle.Broadcast();
}

void AFirstPersonController::Back()
{

	if (bShowingTutorial == true && TutorialSubsystem)
	{
		TutorialSubsystem->HideTutorial();
		bShowingTutorial = false;
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
			ECC_Camera
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
					if (LineTraceHitActor->Implements<UInteractableInterface>())
					{
						TScriptInterface<IInteractableInterface> InteractableScriptInterface = TScriptInterface<
							IInteractableInterface>(LineTraceHitActor);
						if (InteractableScriptInterface && InteractableScriptInterface->IsInteractable() == true)
						{
							FInteractionHint InteractionHint = InteractableScriptInterface->GetHint(this);
							OnShowInteractionHint.Broadcast(InteractionHint);

							InteractableScriptInterface->OnFocus(this);

							return;
						}
					}
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

							InteractableScriptInterface->OnFocus(this);

							return;
						}
					}
				}
			}
		}

		// Hit nothing
		if (LineTraceHitActor && LineTraceHitActor->Implements<UInteractableInterface>())
		{
			TScriptInterface<IInteractableInterface> InteractableScriptInterface = TScriptInterface<
				IInteractableInterface>(LineTraceHitActor);
			if (InteractableScriptInterface && InteractableScriptInterface->IsInteractable() == true)
			{
				InteractableScriptInterface->OnFocusLost(this);
				LineTraceHitActor = nullptr;
			}
		}
		OnHideInteractionHint.Broadcast();
	}
}

void AFirstPersonController::LoadSubsystems()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		AudioSubsystem = LocalPlayer->GetSubsystem<ULocalPlayerAudioSubsystem>();
	}

	GameObjectivesSubsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>();
	if (GameObjectivesSubsystem && AudioSubsystem)
	{
		GameObjectivesSubsystem->OnGameObjectiveNew.AddUObject(AudioSubsystem,
		                                                       &ULocalPlayerAudioSubsystem::OnGameObjectiveNew);
		GameObjectivesSubsystem->OnGameObjectiveCompleted.AddUObject(AudioSubsystem,
		                                                             &ULocalPlayerAudioSubsystem::OnGameObjectiveCompleted);
		GameObjectivesSubsystem->OnGameObjectiveGoalCompleted.AddUObject(
			AudioSubsystem, &ULocalPlayerAudioSubsystem::OnGameObjectiveGoalCompleted);
	}

	TutorialSubsystem = GetWorld()->GetSubsystem<UTutorialSubsystem>();
	if (TutorialSubsystem)
	{
		TutorialSubsystem->OnTutorialShow.AddUObject(this, &AFirstPersonController::OnTutorialShow);
	}
}

void AFirstPersonController::OnTutorialShow(const FText& Title, const FText& Description)
{
	// SetPause(true);
	bShowingTutorial = true;

	if (AudioSubsystem)
	{
		AudioSubsystem->PlaySound2D("Tutorial.Show");
	}
}

void AFirstPersonController::Debug()
{
	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->SetGameObjective("Test.2");
	}

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

void AFirstPersonController::OnPlayerDeath()
{
	bShowMouseCursor = 1;
	SetInputMode(FInputModeUIOnly());
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			if (FirstPersonInputMappingContext)
			{
				InputSystem->RemoveMappingContext(FirstPersonInputMappingContext);
			}
		}
	}
}

void AFirstPersonController::OnLevelReady()
{
	InitializeMappingContexts();
}

void AFirstPersonController::AddItemToInventory(FString ItemName, int32 Quantity)
{
	if (AFirstPersonPlayerState* State = GetPlayerState<AFirstPersonPlayerState>())
	{
		if (State->Inventory)
		{
			State->Inventory->AddItem(ItemName, Quantity);
			if (AudioSubsystem)
			{
				AudioSubsystem->OnItemPickUp();
			}
		}
	}
}

void AFirstPersonController::AddItemToInventory(AActor* Actor, FItemTableRow* Item, int32 Quantity)
{
	if (AFirstPersonPlayerState* State = GetPlayerState<AFirstPersonPlayerState>())
	{
		if (State->Inventory)
		{
			State->Inventory->AddItem(Actor, Item, Quantity);
			if (AudioSubsystem)
			{
				AudioSubsystem->OnItemPickUp();
			}
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
