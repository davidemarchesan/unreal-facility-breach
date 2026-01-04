// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldScanSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UWorldScanSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldScanSubsystem::PostInitialize()
{
	Super::PostInitialize();

	WorldScanManager = Cast<AWorldScanManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AWorldScanManager::StaticClass()));
}

void UWorldScanSubsystem::StartScan(FVector Center)
{
	if (WorldScanManager)
	{
		WorldScanManager->StartScan(Center);
	}
}
