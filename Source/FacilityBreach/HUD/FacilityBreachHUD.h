// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FacilityBreachHUD.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AFacilityBreachHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

private:

	void InitializeOverlayCrosshair();
	
};
