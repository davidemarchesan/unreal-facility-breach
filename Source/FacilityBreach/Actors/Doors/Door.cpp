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

		float t = AnimationTime / AnimationDuration;

		float AdjustedSlidingVelocity = DoorSlidingVelocity * DeltaTime;

		float c = FMath::InterpEaseOut(0.f, 1.f, t, 3.f);
		UE_LOG(LogTemp, Warning, TEXT("t %f | c %f"), t, c);

		AdjustedSlidingVelocity = FMath::InterpEaseInOut(0.f, AdjustedSlidingVelocity, t, 2.f);

		MeshComponent->AddLocalOffset(FVector(AdjustedSlidingVelocity, 0.f, 0.f));

		if (AnimationTime >= AnimationDuration)
		{
			SetDoorState(EDoorState::DOOR_Idle);
		}
	}
}
