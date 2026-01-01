// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "LocalPlayerAudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API ULocalPlayerAudioSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool PlaySound2D(FName SoundName);

	void OnItemPickUp();

private:

	TObjectPtr<const UDataTable> GameAudioDataTable;
	
};
