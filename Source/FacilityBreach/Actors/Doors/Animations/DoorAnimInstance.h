// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FacilityBreach/Actors/Doors/Door.h"
#include "DoorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UDoorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	virtual bool HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) override;

private:

	UPROPERTY(BlueprintReadOnly, Category = "Door", meta = (AllowPrivateAccess = "true"))
	EDoorState DoorState;

	TObjectPtr<ADoor> Door;
};
