// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardCharacter.h"
#include "Components/CapsuleComponent.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"

AEnemyGuardCharacter::AEnemyGuardCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	if (SphereComponent)
	{
		SphereComponent->SetupAttachment(RootComponent);
		SphereComponent->SetGenerateOverlapEvents(true);
		SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	if (AudioComponent)
	{
		AudioComponent->SetupAttachment(RootComponent);
	}

	AudioFeedbackComponent = CreateDefaultSubobject<UAudioComponent>("AudioFeedbackComponent");
	if (AudioFeedbackComponent)
	{
		AudioFeedbackComponent->SetupAttachment(RootComponent);
	}

	AlertFeedbackWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("AlertFeedbackWidgetComponent");
	if (AlertFeedbackWidgetComponent)
	{
		AlertFeedbackWidgetComponent->SetupAttachment(RootComponent);
		AlertFeedbackWidgetComponent->SetVisibility(false);
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AEnemyGuardAIController>(GetController());

	if (SphereComponent)
	{
		SphereComponent->SetSphereRadius(VisionRange);

		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGuardCharacter::OnComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemyGuardCharacter::OnComponentEndOverlap);
	}

	if (AIController)
	{
		if (bStartPatrolOnPlay)
		{
			AIController->StartPatrol(WayPoints, bLoopWayPoints);
		}

		AIController->OnEnterPatrol.AddDynamic(this, &AEnemyGuardCharacter::OnEnterPatrol);
		AIController->OnEnterSuspicious.AddDynamic(this, &AEnemyGuardCharacter::OnEnterSuspicious);
		AIController->OnEnterChase.AddDynamic(this, &AEnemyGuardCharacter::OnEnterChase);
	}

	if (AudioComponent)
	{
		AudioComponent->Play();
	}

	if (AlertFeedbackWidgetComponent)
	{
		AlertFeedbackWidgetComponent->SetVisibility(false);
		AlertFeedbackWidget = Cast<UAlertFeedbackWidget>(AlertFeedbackWidgetComponent->GetUserWidgetObject());
	}
}

void AEnemyGuardCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AFirstPersonCharacter>(OtherActor);
}

void AEnemyGuardCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = nullptr;
}

void AEnemyGuardCharacter::OnEnterPatrol()
{
	if (AlertFeedbackWidgetComponent)
	{
		AlertFeedbackWidgetComponent->SetVisibility(false);
	}
}

void AEnemyGuardCharacter::OnEnterSuspicious()
{
	if (AlertFeedbackWidgetComponent)
	{
		if (AlertFeedbackWidget)
		{
			AlertFeedbackWidget->SetState(EAIGuardState::STATE_Suspicious);
		}
		AlertFeedbackWidgetComponent->SetVisibility(true);
	}

	if (AudioFeedbackComponent && SoundOnSuspicious)
	{
		AudioFeedbackComponent->SetSound(SoundOnSuspicious);
		AudioFeedbackComponent->Play();
	}
}

void AEnemyGuardCharacter::OnEnterChase()
{
	if (AlertFeedbackWidgetComponent)
	{
		AlertFeedbackWidget->SetState(EAIGuardState::STATE_Chase);
		AlertFeedbackWidgetComponent->SetVisibility(true);
	}

	if (AudioFeedbackComponent && SoundOnChase)
	{
		AudioFeedbackComponent->SetSound(SoundOnChase);
		AudioFeedbackComponent->Play();
	}
}

bool AEnemyGuardCharacter::IsPlayerInVision()
{
	if (Player == nullptr)
	{
		return false;
	}

	if (AIController == nullptr)
	{
		return false;
	}

	FVector PlayerLocation = Player->GetActorLocation();

	FVector Direction = PlayerLocation - GetActorLocation();
	Direction.Normalize();

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(), Direction);

	if (DotProduct >= 0)
	{
		// Between 0 and 90 degrees
		// Now check if it's in vision range
		float Cos = FMath::Cos(FMath::DegreesToRadians(VisionAngle * 0.5f));

		if (DotProduct >= Cos)
		{
			FHitResult OutHit;
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				OutHit,
				GetActorLocation(),
				PlayerLocation,
				ECC_Visibility
			);

			if (bHit)
			{
				if (const AActor* HitActor = OutHit.GetActor())
				{
					if (HitActor->IsA(AFirstPersonCharacter::StaticClass()))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void AEnemyGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bSeen = IsPlayerInVision();

	if (bPlayerInVision == false && bSeen == true)
	{
		if (Player && AIController)
		{
			bPlayerInVision = true;
			AIController->OnTargetSeen(Player);
		}
	}
	else if (bPlayerInVision == true && bSeen == false)
	{
		if (AIController)
		{
			bPlayerInVision = false;
			AIController->OnTargetLost();
		}
	}
}

// Called to bind functionality to input
void AEnemyGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
