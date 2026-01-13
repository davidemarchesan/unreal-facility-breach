// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeTest.h"

#include "FacilityBreach/Actors/CheckPoints/CheckPoint.h"
#include "FacilityBreach/Pawns/FirstPersonCharacter.h"
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
				PlayerCharacter->RespawnCharacter(LastCheckPoint->GetActorLocation(), LastCheckPoint->GetActorRotation());
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

void AGameModeTest::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* Controller =  GetWorld()->GetFirstPlayerController())
	{
		PlayerCharacter = Cast<AFirstPersonCharacter>(Controller->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->OnDeath.AddDynamic(this, &AGameModeTest::OnPlayerDeathDelegate);
		}
	}

	CheckPointsSubsystem = GetWorld()->GetSubsystem<UCheckPointsSubsystem>();
}

void AGameModeTest::OnPlayerDeathDelegate()
{
	UE_LOG(LogTemp, Warning, TEXT("AGameModeTest player is dead"));
	OnPlayerDeath.Broadcast();
}
