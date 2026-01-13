// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeTest.h"

#include "FacilityBreach/Actors/CheckPoints/CheckPoint.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
#include "FacilityBreach/PlayerControllers/FirstPersonController.h"
#include "FacilityBreach/Subsystems/World/CheckPointsSubsystem.h"

void AGameModeTest::RespawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("AGameModeTest::RespawnPlayer"));
	if (CheckPointsSubsystem)
	{
		if (ACheckPoint* LastCheckPoint = CheckPointsSubsystem->GetLastCheckPoint())
		{
			if (PlayerCharacter)
			{
				PlayerCharacter->RespawnCharacter(LastCheckPoint->GetActorLocation(),
				                                  LastCheckPoint->GetActorRotation());
				return;
			}
		}
	}

	// Player did not reach any checkpoints
	// Respawn from player start
	if (PlayerCharacter)
	{
		PlayerCharacter->RespawnCharacter();
	}
}

void AGameModeTest::CheckForLevelLoaded()
{
	// Avoid to broadcast multiple times
	if (bReadyBroadcasted == true)
	{
		return;
	}
	
	if (bReady == true && bControllerReady == true)
	{
		OnLevelReady.Broadcast();
		bReadyBroadcasted = true;
	}
}

void AGameModeTest::OnControllerReady()
{
	bControllerReady = true;
	CheckForLevelLoaded();
}

void AGameModeTest::BeginPlay()
{
	Super::BeginPlay();

	if (AFirstPersonController* Controller = Cast<AFirstPersonController>(GetWorld()->GetFirstPlayerController()))
	{

		// Check if controller is ready
		if (Controller->IsReady() == true)
		{
			bControllerReady = true;
		}
		else
		{
			Controller->OnControllerReady.AddUObject(this, &AGameModeTest::OnControllerReady);
		}

		PlayerCharacter = Cast<AFirstPersonCharacter>(Controller->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->OnDeath.AddDynamic(this, &AGameModeTest::OnPlayerDeathDelegate);
		}
	}

	CheckPointsSubsystem = GetWorld()->GetSubsystem<UCheckPointsSubsystem>();

	bReady = true;
	
	CheckForLevelLoaded();
}

void AGameModeTest::OnPlayerDeathDelegate()
{
	UE_LOG(LogTemp, Warning, TEXT("AGameModeTest player is dead"));
	OnPlayerDeath.Broadcast();
}
