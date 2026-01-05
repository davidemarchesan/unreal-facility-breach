// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTest.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API AGameModeTest : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION() void OnPlayerDeath();
	
};
