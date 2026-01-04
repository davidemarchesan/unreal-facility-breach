// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Actors/ScanSphere/WorldScanManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldScanSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UWorldScanSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void PostInitialize() override;

	void StartScan(FVector Center);

private:

	TObjectPtr<AWorldScanManager> WorldScanManager;
	
};
