// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanSphere.h"

#include "Engine/OverlapResult.h"

AScanSphere::AScanSphere()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		RootComponent = SceneComponent;
	}

	SetHidden(true);
	SetCanBeDamaged(false);
}

void AScanSphere::StartScan(float InSpeed, float InMaxRadius)
{
	Speed = InSpeed;
	MaxRadius = InMaxRadius;

	StartTime = GetWorld()->GetTimeSeconds();
	bScanning = true;
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

	// Skip frames
	// No need to check overlaps every frame
	AccumulatedTime += DeltaTime;
	if (AccumulatedTime < UpdateInterval)
	{
		return;
	}
	AccumulatedTime = 0.f;

	const float ElapsedTime = (GetWorld()->GetTimeSeconds() - StartTime);
	const float Radius = ElapsedTime * Speed;

	// Start Debug
	// UE_LOG(LogTemp, Warning, TEXT("ElapsedTime: %f | Radius: %f"), ElapsedTime, Radius);
	// DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 16, FColor::Red);
	// End Debug

	// Apparently physic radius is not the same of visual
	float OverlapRadius = FMath::Clamp(Radius - OverlapMargin, 0.f, MaxRadius);
	if (OverlapRadius <= 0.f)
	{
		return;
	}

	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(OverlapRadius);

	GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		Sphere
	);

	if (OutOverlaps.Num() > 0)
	{
		for (FOverlapResult& Overlap : OutOverlaps)
		{
			if (AEnemyGuardCharacter* Enemy = Cast<AEnemyGuardCharacter>(Overlap.GetActor()))
			{
				if (AlreadyDetectedEnemies.Find(Enemy) == INDEX_NONE)
				{
					Enemy->SetDetected();
					AlreadyDetectedEnemies.Push(Enemy);
				}
			}
		}
	}

	if (Radius >= MaxRadius)
	{
		bScanning = false;
		Destroy();
	}
}
