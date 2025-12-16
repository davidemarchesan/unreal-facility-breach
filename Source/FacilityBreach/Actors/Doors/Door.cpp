// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPlayerBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnPlayerEndOverlap);
	}
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::SetDoorState(EDoorState NewState)
{
	DoorState = NewState;
	if (NewState != EDoorState::DOOR_Idle)
	{
		AnimationTime = 0.f;
	}
}

void ADoor::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	SetDoorState(EDoorState::DOOR_Opening);
}

void ADoor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetDoorState(EDoorState::DOOR_Closing);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorState != EDoorState::DOOR_Idle)
	{
		AnimationTime += DeltaTime;

		float t = FMath::Clamp(AnimationTime / AnimationDuration, 0.f, 1.f);
		float AdjustedSlidingVelocity = t < 0.5f ? FMath::InterpCircularOut(0.f, DoorSlidingVelocity, t * 2) : FMath::InterpCircularIn(DoorSlidingVelocity, 0.f, (t * 2) - 1.f);
		
		MeshComponent->AddLocalOffset(FVector(AdjustedSlidingVelocity * DeltaTime, 0.f, 0.f));

		if (AnimationTime >= AnimationDuration)
		{
			SetDoorState(EDoorState::DOOR_Idle);
		}
	}
}
