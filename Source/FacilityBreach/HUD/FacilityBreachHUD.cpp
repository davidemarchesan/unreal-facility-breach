// Fill out your copyright notice in the Description page of Project Settings.


#include "FacilityBreachHUD.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "FacilityBreach/GameModes/GameModeTest.h"
#include "FacilityBreach/Pawns/Ability/AbilityComponent.h"
#include "FacilityBreach/PlayerControllers/FirstPersonController.h"
#include "FacilityBreach/PlayerStates/FirstPersonPlayerState.h"
#include "FacilityBreach/Subsystems/World/GameObjectivesSubsystem.h"
#include "FacilityBreach/Subsystems/World/TutorialSubsystem.h"
#include "FacilityBreach/UI/Slate/Overlays/Abilities/AbilitiesOverlay.h"
#include "FacilityBreach/UI/Slate/Overlays/EndGame/EndGameOverlay.h"

#include "Widgets/SOverlay.h"

void AFacilityBreachHUD::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AFirstPersonCharacter>(GetOwningPawn());

	GameMode = Cast<AGameModeTest>(GetWorld()->GetAuthGameMode());

	InitializeDelegatesSub();

	InitializeOverlays();
}

void AFacilityBreachHUD::InitializeDelegatesSub()
{
	InitializeDelegatesAbilities();
	InitializeDelegatesInteractables();
	InitializeDelegatesGameObjectives();
	InitializeDelegatesInventory();
	InitializeDelegatesTutorial();
	InitializeDelegatesEndGame();
	InitializeDelegatesLoading();
}

void AFacilityBreachHUD::InitializeDelegatesAbilities()
{
	if (CharacterOwner == nullptr)
	{
		return;
	}

	UAbilityComponent* AbilityComponent = CharacterOwner->GetAbilityComponent();
	if (AbilityComponent == nullptr)
	{
		return;
	}

	AbilityComponent->OnAbilityCooldownStart.AddDynamic(this, &AFacilityBreachHUD::OnAbilityCooldownStart);
	AbilityComponent->OnAbilityCooldownEnd.AddDynamic(this, &AFacilityBreachHUD::OnAbilityCooldownEnd);
	AbilityComponent->OnAbilityChargesChange.AddDynamic(this, &AFacilityBreachHUD::OnAbilityChargesChange);
}

void AFacilityBreachHUD::OnAbilityCooldownStart(EAbilityType AbilityType, float Seconds)
{
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityCooldownStart(AbilityType, Seconds);
	}
}

void AFacilityBreachHUD::OnAbilityCooldownEnd(EAbilityType AbilityType)
{
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityCooldownEnd(AbilityType);
	}
}

void AFacilityBreachHUD::OnAbilityChargesChange(EAbilityType AbilityType, int32 Charges)
{
	if (AbilitiesOverlay)
	{
		AbilitiesOverlay->OnAbilityChargesChange(AbilityType, Charges);
	}
}

void AFacilityBreachHUD::InitializeDelegatesInteractables()
{
	AFirstPersonController* Controller = Cast<AFirstPersonController>(GetOwningPlayerController());
	if (Controller)
	{
		Controller->OnShowInteractionHint.AddDynamic(this, &AFacilityBreachHUD::OnShowInteractionHint);
		Controller->OnHideInteractionHint.AddDynamic(this, &AFacilityBreachHUD::OnHideInteractionHint);
	}
}

void AFacilityBreachHUD::OnShowInteractionHint(FInteractionHint Hint)
{
	if (InteractablesOverlay)
	{
		InteractablesOverlay->OnShowInteractionHint(Hint);
	}
}

void AFacilityBreachHUD::OnHideInteractionHint()
{
	if (InteractablesOverlay)
	{
		InteractablesOverlay->OnHideInteractionHint();
	}
}

void AFacilityBreachHUD::InitializeDelegatesGameObjectives()
{
	if (UGameObjectivesSubsystem* Subsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>())
	{
		Subsystem->OnGameObjectiveNew.AddUObject(this, &AFacilityBreachHUD::OnGameObjectiveNew);
		Subsystem->OnGameObjectiveUpdate.AddUObject(this, &AFacilityBreachHUD::OnGameObjectiveUpdate);
		Subsystem->OnGameObjectiveCompleted.AddUObject(this, &AFacilityBreachHUD::OnGameObjectiveCompleted);
	}
}

void AFacilityBreachHUD::OnGameObjectiveNew(const FGameObjectiveState& CurrentObjectiveState)
{
	if (GameObjectivesOverlay)
	{
		GameObjectivesOverlay->OnGameObjectiveNew(CurrentObjectiveState);
	}
}

void AFacilityBreachHUD::OnGameObjectiveUpdate(const FGameObjectiveState& CurrentObjectiveState)
{
	if (GameObjectivesOverlay)
	{
		GameObjectivesOverlay->OnGameObjectiveUpdate(CurrentObjectiveState);
	}
}

void AFacilityBreachHUD::OnGameObjectiveCompleted(const FGameObjectiveState& CurrentObjectiveState)
{
	if (GameObjectivesOverlay)
	{
		GameObjectivesOverlay->OnGameObjectiveCompleted(CurrentObjectiveState);
	}
}

void AFacilityBreachHUD::InitializeDelegatesInventory()
{
	if (AFirstPersonController* Controller = Cast<AFirstPersonController>(GetOwningPlayerController()))
	{

		Controller->OnInventoryToggle.AddUObject(this, &AFacilityBreachHUD::OnInventoryToggle);
		
		if (AFirstPersonPlayerState* PlayerState = Controller->GetPlayerState<
			AFirstPersonPlayerState>())
		{
			if (PlayerState->Inventory)
			{
				PlayerState->Inventory->OnAddItem.AddUObject(this, &AFacilityBreachHUD::OnInventoryAddItem);
			}
		}
	}
}

void AFacilityBreachHUD::OnInventoryToggle()
{
	if (InventoryOverlay.IsValid())
	{
		InventoryOverlay->OnToggle();
	}
}

void AFacilityBreachHUD::OnInventoryAddItem(FString ItemName)
{
	if (InventoryOverlay.IsValid())
	{
		InventoryOverlay->OnAddItem(ItemName);
	}
}

void AFacilityBreachHUD::InitializeDelegatesTutorial()
{
	if (UTutorialSubsystem* Subsystem = GetWorld()->GetSubsystem<UTutorialSubsystem>())
	{
		Subsystem->OnTutorialShow.AddUObject(this, &AFacilityBreachHUD::OnTutorialShow);
		Subsystem->OnTutorialHide.AddUObject(this, &AFacilityBreachHUD::OnTutorialHide);
	}
}

void AFacilityBreachHUD::OnTutorialShow(const FText& Title, const FText& Description)
{
	if (TutorialOverlay.IsValid())
	{
		TutorialOverlay->OnShow(Title, Description);
	}
}

void AFacilityBreachHUD::OnTutorialHide()
{
	if (TutorialOverlay.IsValid())
	{
		TutorialOverlay->OnHide();
	}
}

void AFacilityBreachHUD::InitializeDelegatesEndGame()
{
	if (GameMode)
	{
		GameMode->OnPlayerDeath.AddUObject(this, &AFacilityBreachHUD::OnPlayerDeath);
	}
}

void AFacilityBreachHUD::OnPlayerDeath()
{
	if (EndGameOverlay.IsValid())
	{
		EndGameOverlay->SetVisibility(EVisibility::Visible);
	}
}

void AFacilityBreachHUD::InitializeDelegatesLoading()
{
	if (GameMode)
	{
		GameMode->OnLevelReady.AddUObject(this, &AFacilityBreachHUD::OnLevelReady);
	}
}

void AFacilityBreachHUD::OnLevelReady()
{
	if (LoadingOverlay.IsValid())
	{
		LoadingOverlay->FadeOut();
	}
}

void AFacilityBreachHUD::InitializeOverlays()
{
	if (GEngine && GEngine->GameViewport)
	{
		InitializeOverlayCrosshair();
		InitializeOverlayAbilities();
		InitializeOverlayInteractables();
		InitializeOverlayGameObjectives();
		InitializeOverlayInventory();
		InitializeOverlayTutorial();
		InitializeOverlayEndGame();
		InitializeOverlayLoading();
	}
}

void AFacilityBreachHUD::InitializeOverlayCrosshair()
{
	const TSharedRef<SOverlay> Overlay = SNew(SOverlay);

	Overlay->AddSlot()
	       .HAlign(HAlign_Center)
	       .VAlign(VAlign_Center)
	[

		SNew(SBorder)
		.BorderImage(new FSlateRoundedBoxBrush(FLinearColor::White, 2.f, FLinearColor::Black, 1.f))
		[

			SNew(SBox)
			.HeightOverride(2.f)
			.WidthOverride(2.f)

		]

	];

	GEngine->GameViewport->AddViewportWidgetContent(Overlay);
}

void AFacilityBreachHUD::InitializeOverlayAbilities()
{
	AbilitiesOverlay = SNew(SAbilitiesOverlay);

	if (AbilitiesOverlay && CharacterOwner)
	{
		if (UAbilityComponent* AbilityComponent = CharacterOwner->GetAbilityComponent())
		{
			AbilitiesOverlay->InitializeAbilities(AbilityComponent->AbilitiesDataTable);
		}
		GEngine->GameViewport->AddViewportWidgetContent(AbilitiesOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayInteractables()
{
	InteractablesOverlay = SNew(SInteractablesOverlay);

	if (InteractablesOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(InteractablesOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayGameObjectives()
{
	GameObjectivesOverlay = SNew(SGameObjectivesOverlay);

	if (GameObjectivesOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(GameObjectivesOverlay.ToSharedRef());

		if (UGameObjectivesSubsystem* Subsystem = GetWorld()->GetSubsystem<UGameObjectivesSubsystem>())
		{
			const FGameObjectiveState& CurrentObjectiveState = Subsystem->GetCurrentObjectiveState();
			if (GameObjectivesOverlay)
			{
				GameObjectivesOverlay->OnGameObjectiveNew(CurrentObjectiveState);
			}
		}
	}
}

void AFacilityBreachHUD::InitializeOverlayInventory()
{
	InventoryOverlay = SNew(SInventoryOverlay);

	if (InventoryOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(InventoryOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayTutorial()
{
	TutorialOverlay = SNew(STutorialOverlay);

	if (TutorialOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(TutorialOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayEndGame()
{
	EndGameOverlay = SNew(SEndGameOverlay)
		.OnRespawn_Lambda([this]() -> FReply
		{
			if (GameMode)
			{
				GameMode->RespawnPlayer();
			}
			return FReply::Handled();
		});
	EndGameOverlay->SetVisibility(EVisibility::Collapsed);

	if (EndGameOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(EndGameOverlay.ToSharedRef());
	}
}

void AFacilityBreachHUD::InitializeOverlayLoading()
{
	LoadingOverlay = SNew(SLoadingOverlay);

	if (LoadingOverlay)
	{
		GEngine->GameViewport->AddViewportWidgetContent(LoadingOverlay.ToSharedRef());
	}
}
