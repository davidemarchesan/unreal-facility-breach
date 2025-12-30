// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FacilityBreach/AIControllers/EnemyGuardAIController.h"
#include "FacilityBreach/UI/Slate/Widgets/AlertFeedback.h"
#include "AlertFeedbackWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UAlertFeedbackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetState(EAIGuardState State);

private:
	
	TSharedPtr<SAlertFeedback> SlateWidget;
	
};
