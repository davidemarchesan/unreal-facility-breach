// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorAnimInstance.h"

void UDoorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Door = Cast<ADoor>(GetOwningActor());
	
}

void UDoorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Door == nullptr)
	{
		Door = Cast<ADoor>(GetOwningActor());
	}

	if (Door == nullptr) return; // We tried

	DoorState = Door->GetDoorState();
	
}

bool UDoorAnimInstance::HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent)
{

	if (AnimNotifyEvent.NotifyName == "Door_OpenCompleted")
	{
		if (Door)
		{
			Door->OnOpenCompleted();
		}
	}

	if (AnimNotifyEvent.NotifyName == "Door_CloseCompleted")
	{
		if (Door)
		{
			Door->OnCloseCompleted();
		}
	}

	return Super::HandleNotify(AnimNotifyEvent);
}
