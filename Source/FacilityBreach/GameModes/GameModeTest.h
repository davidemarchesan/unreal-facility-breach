// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Subsystems/World/CheckPointsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTest.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeath)
DECLARE_MULTICAST_DELEGATE(FOnPlayerRespawn)

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AGameModeTest : public AGameModeBase
{
	GENERATED_BODY()

private:

	UFUNCTION() void OnPlayerDeathDelegate();

	AFirstPersonCharacter* PlayerCharacter;
	UCheckPointsSubsystem* CheckPointsSubsystem;

protected:

	virtual void BeginPlay() override;

public:

	void RespawnPlayer();

	FOnPlayerDeath OnPlayerDeath;
	FOnPlayerRespawn OnPlayerRespawn;
	
};
