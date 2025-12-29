// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "WayPoint.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AWayPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, Category="Patrol")
	float StayInPositionTime = 0.f;

	UPROPERTY(EditInstanceOnly, Category = "Patrol", meta = (ToolTip="AI will have the same facing direction of this WayPoint when StayInPositionTime is greater than zero."))
	bool bFaceWayPointDirection = true;
	
};
