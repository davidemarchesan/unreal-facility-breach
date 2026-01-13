// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/Subsystems/World/CheckPointsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTest.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeath)
DECLARE_MULTICAST_DELEGATE(FOnPlayerRespawn)

DECLARE_MULTICAST_DELEGATE(FOnLevelReady)

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

	bool bReady = false;
	bool bControllerReady = false;
	bool bReadyBroadcasted = false;

	void CheckForLevelLoaded();

	void OnControllerReady();

protected:

	virtual void BeginPlay() override;

public:

	void RespawnPlayer();

	FOnPlayerDeath OnPlayerDeath;
	FOnPlayerRespawn OnPlayerRespawn;

	FOnLevelReady OnLevelReady;
	
};
