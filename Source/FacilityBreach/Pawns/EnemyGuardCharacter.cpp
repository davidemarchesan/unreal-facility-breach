// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGuardCharacter.h"

#include "Components/CapsuleComponent.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"
#include "FacilityBreach/PostProcess/Stencils/Stencils.h"

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

		TArray<AActor*> OverlappingActors;
		TSubclassOf<AFirstPersonCharacter> ClassFilter = AFirstPersonCharacter::StaticClass();
		SphereComponent->GetOverlappingActors(OverlappingActors, ClassFilter);

		if (OverlappingActors.Num() > 0)
		{
			Player = Cast<AFirstPersonCharacter>(OverlappingActors[0]);
		}
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

	SetPatrolSpeed();

	GameObjectivesSubsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>();
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

	SetPatrolSpeed();
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

	SetPatrolSpeed();
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

	SetChaseSpeed();
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

bool AEnemyGuardCharacter::IsPlayerBehind()
{
	if (Player == nullptr)
	{
		return false;
	}

	FVector PlayerLocation = Player->GetActorLocation();

	FVector Direction = PlayerLocation - GetActorLocation();
	Direction.Normalize();

	const float DotProduct = FVector::DotProduct(GetActorForwardVector(), Direction);

	if (DotProduct < 0)
	{
		// Between 90 and 180 degrees
		float Cos = FMath::Cos(FMath::DegreesToRadians(40.f * 0.5f)) * -1.f; // -1.f = Negative

		if (DotProduct <= Cos)
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

void AEnemyGuardCharacter::SetPatrolSpeed()
{
	SetSpeed(StandardSpeed);
}

void AEnemyGuardCharacter::SetChaseSpeed()
{
	SetSpeed(ChaseSpeed);
}

void AEnemyGuardCharacter::SetSpeed(float InSpeed)
{
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->MaxFlySpeed = InSpeed;
	}
}

void AEnemyGuardCharacter::Deactivate()
{

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetSimulatePhysics(true);
	}
	
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkeletalMeshComponent->SetSimulatePhysics(true);
		SkeletalMeshComponent->bPauseAnims = true;
		
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->DisableMovement();
	}

	if (AudioComponent && SoundOnDeactivated)
	{
		AudioComponent->SetSound(SoundOnDeactivated);
		AudioComponent->Play();
	}

	bDeactivated = true;
	
	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->Emit(this, GameObjectivesSubsystem->Tag_Action_Enemy_Deactivated);
	}
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

void AEnemyGuardCharacter::SetDetected()
{
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetCustomDepthStencilValue(static_cast<int32>(EStencilType::STENCIL_Detected));
		SkeletalMeshComponent->SetRenderCustomDepth(true);

		GetWorldTimerManager().ClearTimer(DetectedTimerHandle);
		GetWorldTimerManager().SetTimer(DetectedTimerHandle, this, &AEnemyGuardCharacter::SetUndetected, 2.f);
	}

	if (AudioFeedbackComponent && SoundOnDetected)
	{
		AudioFeedbackComponent->SetSound(SoundOnDetected);
		AudioFeedbackComponent->Play();
	}
}

void AEnemyGuardCharacter::SetUndetected()
{
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}
}

FInteractionHint AEnemyGuardCharacter::GetHint(APlayerController* PlayerController)
{
	if (IsPlayerBehind() && bDeactivated == false)
	{
		return FInteractionHint(FText::FromString("Deactivate"));
	}
	else
	{
		return FInteractionHint();
	}
}

void AEnemyGuardCharacter::OnInteract(APlayerController* PlayerController)
{
	if (bDeactivated == true)
	{
		return;
	}
	
	if (IsPlayerBehind())
	{
		Deactivate();
	}
}

void AEnemyGuardCharacter::OnFocus(APlayerController* PlayerController)
{
	if (bDeactivated == true)
	{
		return;
	}
	
	if (IsPlayerBehind())
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
		{
			SkeletalMeshComponent->SetCustomDepthStencilValue(static_cast<int32>(EStencilType::STENCIL_Outline));
			SkeletalMeshComponent->SetRenderCustomDepth(true);
		}
	}
}

void AEnemyGuardCharacter::OnFocusLost(APlayerController* PlayerController)
{
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		SkeletalMeshComponent->SetRenderCustomDepth(false);
	}
}

void AEnemyGuardCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}
