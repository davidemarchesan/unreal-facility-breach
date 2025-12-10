// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "FirstPersonPawn.generated.h"

UCLASS()
class FACILITYBREACH_API AFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFirstPersonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void RequestMove(FVector2D InDirection);

private:

	// Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CollisionComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	// Movement and Look
	void UpdateLocation(float DeltaTime);
	
	FVector2D MovementDirection;

};
