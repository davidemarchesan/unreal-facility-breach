// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FacilityBreach/Input/Configs/FirstPersonInputConfig.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Pawns/FirstPersonPawn.h"
#include "FacilityBreach/UI/Slate/Styles/FacilityBreachStyle.h"

void AFirstPersonController::BeginPlay()
{
	Super::BeginPlay();

	InitializeMappingContexts();

	FirstPersonCharacter = Cast<AFirstPersonCharacter>(GetPawn());
	if (FirstPersonCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FirstPersonController: No Character found!"));
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
