// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScanSphere.generated.h"

UCLASS()
class FACILITYBREACH_API AScanSphere : public AActor
{
	GENERATED_BODY()
	
public:
	
	AScanSphere();

	virtual void Tick(float DeltaTime) override;

	void StartScan();

protected:
	
	virtual void BeginPlay() override;

private:

	bool bScanning = false;

	float StartTime;

	float GrowingSpeed = 400.f;

	float MaxRadius = 500.f;
	

};
