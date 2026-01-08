// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"

#include "Components/BoxComponent.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"

void UCheckPointTrigger::Execute()
{
	return;
}

void UCheckPointTrigger_Tutorial::Execute()
{
	if (Tutorial == nullptr)
	{
		return;
	}
	
	if (UTutorialSubsystem* Subsystem = GetWorld()->GetSubsystem<UTutorialSubsystem>())
	{
		Subsystem->ShowTutorial(Tutorial->Title, Tutorial->Description);
	}
}

void UCheckPointTrigger_GameObjective::Execute()
{
	if (GameObjective == nullptr)
	{
		return;
	}

	if (UGameObjectivesSubsystem* Subsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>())
	{
		Subsystem->SetGameObjective(GameObjective);
	}
}

void ACheckPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTags;
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	if (UBoxComponent* BoxCollisionComponent = CastChecked<UBoxComponent>(GetCollisionComponent()))
	{
		BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnComponentBeginOverlap);
	}

	GameObjectivesSubsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>();
}

void ACheckPoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                          bool bFromSweep, const FHitResult& SweepResult)
{

	if (bFireOnce == true && bFired == true)
	{
		return;
	}

	if (GameObjectivesSubsystem)
	{
		GameObjectivesSubsystem->Emit(this, GameObjectivesSubsystem->Tag_Action_Overlap_Begin);
	}

	if (Triggers.Num() > 0)
	{
		for (TObjectPtr<UCheckPointTrigger>& Trigger : Triggers)
		{
			Trigger->Execute();
		}
	}

	bFired = true;
	
}
