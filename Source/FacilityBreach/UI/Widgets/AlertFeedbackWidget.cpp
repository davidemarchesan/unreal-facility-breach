// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertFeedbackWidget.h"

TSharedRef<SWidget> UAlertFeedbackWidget::RebuildWidget()
{
	SlateWidget = SNew(SAlertFeedback);
	return SlateWidget.ToSharedRef();
}

void UAlertFeedbackWidget::SetState(EAIGuardState State)
{

	if (SlateWidget.IsValid() == false)
	{
		return;
	}
	
	switch (State)
	{
	case EAIGuardState::STATE_Suspicious:
		SlateWidget->SetIcon("General.Awareness.Icon", FLinearColor::Yellow);
		break;
	case EAIGuardState::STATE_Chase:
		SlateWidget->SetIcon("General.Awareness.Icon", FLinearColor::Red);
		break;
	default:
		break;
	}
}
