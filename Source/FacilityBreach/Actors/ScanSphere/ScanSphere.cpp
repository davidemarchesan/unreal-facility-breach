// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanSphere.h"

#include "Engine/OverlapResult.h"

AScanSphere::AScanSphere()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AScanSphere::StartScan()
{
	
	StartTime = GetWorld()->GetTimeSeconds();
	bScanning = true;

	// StartTime = GetWorld()->GetTimeSeconds();
	// bScanStarted = true;
	//
	// TArray<FOverlapResult> OutOverlaps;
	//
	// FCollisionShape Sphere = FCollisionShape::MakeSphere(500.f);
	//
	// GetWorld()->OverlapMultiByChannel(
	// 	OutOverlaps,
	// 	GetActorLocation(),
	// 	FQuat::Identity,
	// 	ECC_Pawn,
	// 	Sphere
	// );
	//
	// if (OutOverlaps.Num() > 0)
	// {
	// 	for (FOverlapResult& Overlap : OutOverlaps)
	// 	{
	// 		if (AActor* ActorHit = Overlap.GetActor())
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("Actor hit by overlap sphere %s"), *ActorHit->GetName());
	// 		}
	// 	}
	// }
}

void AScanSphere::BeginPlay()
{
	Super::BeginPlay();
}

void AScanSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bScanning == false)
	{
		return;
	}

	const float ElapsedTime = (GetWorld()->GetTimeSeconds() - StartTime);
	
	const float Radius = ElapsedTime * GrowingSpeed;

	UE_LOG(LogTemp, Warning, TEXT("ElapsedTime: %f | Radius: %f"), ElapsedTime, Radius);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, FColor::Red);

	if (Radius >= MaxRadius)
	{
		bScanning = false;
	}
}
