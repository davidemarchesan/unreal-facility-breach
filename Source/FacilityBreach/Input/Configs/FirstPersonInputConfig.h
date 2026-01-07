// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FirstPersonInputConfig.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UFirstPersonInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Dash;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Scan;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Debug;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Tab;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_SecondaryAction;
	
};
