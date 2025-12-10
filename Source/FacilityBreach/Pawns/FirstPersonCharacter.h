// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"

class UCameraComponent;

UCLASS()
class FACILITYBREACH_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AFirstPersonCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera
	UPROPERTY(EditAnywhere)
	UCameraComponent* FirstPersonCameraComponent;

};
