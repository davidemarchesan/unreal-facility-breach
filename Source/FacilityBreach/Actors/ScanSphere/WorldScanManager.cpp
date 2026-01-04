// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldScanManager.h"

#include "ScanSphere.h"
#include "Kismet/GameplayStatics.h"

AWorldScanManager::AWorldScanManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldScanManager::BeginPlay()
{
	Super::BeginPlay();

	PostProcessVolume = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
	if (PostProcessVolume)
	{
		InitializePostProcessMaterials();
	}
}

void AWorldScanManager::InitializePostProcessMaterials()
{
	if (PostProcessVolume == nullptr)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("AWorldScanManager::InitializePostProcessMaterials(): could not find post process volume"));
		return;
	}

	ScanPostProcessDynamicMaterial = UMaterialInstanceDynamic::Create(ScanPostProcessMaterialInstance, this);
	if (ScanPostProcessDynamicMaterial == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create post process dynamic material ScanPostProcessMaterial"));
	}

	PostProcessVolume->Settings.WeightedBlendables.Array.Add(
		FWeightedBlendable(1.f, ScanPostProcessDynamicMaterial)
	);

}

void AWorldScanManager::OnSphereDestroyed(AActor* DestroyedActor)
{
	if (ScanPostProcessDynamicMaterial)
	{
		ScanPostProcessDynamicMaterial->SetScalarParameterValue("StartTime", 0.f);
	}
}

void AWorldScanManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldScanManager::StartScan(FVector Center)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning sphere at %s"), *Center.ToString());
	if (AScanSphere* ScanSphere = GetWorld()->SpawnActor<AScanSphere>(Center, FRotator::ZeroRotator))
	{
		ScanSphere->OnDestroyed.AddDynamic(this, &AWorldScanManager::OnSphereDestroyed);
		ScanSphere->StartScan(Speed, MaxRadius);
		if (ScanPostProcessDynamicMaterial)
		{
			ScanPostProcessDynamicMaterial->SetVectorParameterValue("Center", Center);
			ScanPostProcessDynamicMaterial->SetVectorParameterValue("Color", Color);
			ScanPostProcessDynamicMaterial->SetScalarParameterValue("Thickness", Thickness);
			ScanPostProcessDynamicMaterial->SetScalarParameterValue("Speed", Speed);
			ScanPostProcessDynamicMaterial->SetScalarParameterValue("MaxRadius", MaxRadius);
			ScanPostProcessDynamicMaterial->SetScalarParameterValue("StartTime", GetWorld()->GetTimeSeconds());
		}
	}
}
