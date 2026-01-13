// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CheckPointsSubsystem.generated.h"

class ACheckPoint;

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UCheckPointsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:

	ACheckPoint* LastCheckPoint;

protected:

public:

	ACheckPoint* GetLastCheckPoint() const { return LastCheckPoint; }
	void SetLastCheckPoint(ACheckPoint* CheckPoint) { LastCheckPoint = CheckPoint; }
	
};
