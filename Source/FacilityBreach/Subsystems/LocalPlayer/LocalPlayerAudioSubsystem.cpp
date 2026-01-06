// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalPlayerAudioSubsystem.h"

#include "FacilityBreach/Audio/Audio.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULocalPlayerAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftObjectPath GameAudioDataPath(TEXT("/Game/Audio/DT_GameAudio.DT_GameAudio"));

	GameAudioDataTable = Cast<UDataTable>(GameAudioDataPath.TryLoad());
	if (GameAudioDataTable == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not load GameAudioDataTable %s"), *GameAudioDataPath.ToString());
	}
}

bool ULocalPlayerAudioSubsystem::PlaySound2D(FName SoundName)
{
	if (GameAudioDataTable == nullptr)
	{
		return false;
	}

	if (FGameAudioTableRow* GameAudioTableRow = GameAudioDataTable->FindRow<FGameAudioTableRow>(SoundName, "Sound"))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), GameAudioTableRow->Sound);
		return true;
	}

	return false;
}

void ULocalPlayerAudioSubsystem::OnItemPickUp()
{
	PlaySound2D("PickUp");
}

void ULocalPlayerAudioSubsystem::OnGameObjectiveCompleted(FGameObjectiveState CurrentObjectiveState)
{
	PlaySound2D("GameObjective.Completed");
}

void ULocalPlayerAudioSubsystem::OnGameObjectiveGoalCompleted(FGameObjectiveState CurrentObjectiveState)
{
	PlaySound2D("GameObjective.Goal.Completed");
}
