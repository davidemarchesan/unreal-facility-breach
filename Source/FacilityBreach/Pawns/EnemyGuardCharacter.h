// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Actors/WayPoints/WayPoint.h"
#include "GameFramework/Character.h"
#include "EnemyGuardCharacter.generated.h"

UCLASS()
class FACILITYBREACH_API AEnemyGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TArray<TObjectPtr<AWayPoint>> GetWayPoints() const { return WayPoints; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;

	UPROPERTY(Category=AI, EditInstanceOnly)
	TArray<TObjectPtr<AWayPoint>> WayPoints;

private:

};
