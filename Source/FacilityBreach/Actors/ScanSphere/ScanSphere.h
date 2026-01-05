// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Pawns/EnemyGuardCharacter.h"
#include "GameFramework/Actor.h"
#include "ScanSphere.generated.h"

UCLASS()
class FACILITYBREACH_API AScanSphere : public AActor
{
	GENERATED_BODY()
	
public:
	
	AScanSphere();

	virtual void Tick(float DeltaTime) override;

	void StartScan(float InSpeed, float InMaxRadius);

protected:
	
	virtual void BeginPlay() override;

private:

	bool bScanning = false;

	float StartTime;

	float Speed = 400.f;

	float MaxRadius = 500.f;
	float OverlapMargin = 800.f;

	TArray<AEnemyGuardCharacter*> AlreadyDetectedEnemies;

	float AccumulatedTime = 0.f;
	float UpdateInterval = 0.05f; // Seconds

};
