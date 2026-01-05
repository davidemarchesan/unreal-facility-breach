// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeTest.h"

#include "FacilityBreach/Pawns/FirstPersonCharacter.h"

void AGameModeTest::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* Controller =  GetWorld()->GetFirstPlayerController())
	{
		AFirstPersonCharacter* PlayerCharacter = Cast<AFirstPersonCharacter>(Controller->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->OnDeath.AddDynamic(this, &AGameModeTest::OnPlayerDeath);
		}
	}
}

void AGameModeTest::OnPlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("AGameModeTest player is dead"));
}
