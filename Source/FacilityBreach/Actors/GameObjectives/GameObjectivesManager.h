// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObjectives.h"
#include "GameFramework/Actor.h"
#include "GameObjectivesManager.generated.h"

UCLASS()
class FACILITYBREACH_API AGameObjectivesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameObjectivesManager();

	virtual void Tick(float DeltaTime) override;

	TArray<UGameObjective*> GetGameObjectives() { return GameObjectives; }

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameObjectives", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UGameObjective>> GameObjectives;

};
