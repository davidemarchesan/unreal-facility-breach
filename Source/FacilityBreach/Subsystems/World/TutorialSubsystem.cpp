// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialSubsystem.h"

void UTutorialSubsystem::ShowTutorial(const FText& Title, const FText& Description)
{
	OnTutorialShow.Broadcast(Title, Description);;
}

void UTutorialSubsystem::HideTutorial()
{
	OnTutorialHide.Broadcast();
}
