// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldScanManager.generated.h"

UCLASS()
class FACILITYBREACH_API AWorldScanManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldScanManager();

	virtual void Tick(float DeltaTime) override;

	void StartScan(FVector Center);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Scan")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, Category = "Scan")
	float Thickness = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "Scan")
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Scan")
	float MaxRadius = 500.f;

private:

	TObjectPtr<APostProcessVolume> PostProcessVolume;

	void InitializePostProcessMaterials();

	TObjectPtr<UMaterialInstanceDynamic> ScanPostProcessDynamicMaterial;

	UPROPERTY(Category="Materials", EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> ScanPostProcessMaterialInstance;

	UFUNCTION() void OnSphereDestroyed(AActor* DestroyedActor);
	

};
