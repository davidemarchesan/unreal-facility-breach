// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"
#include "ConeComponent.generated.h"

/**
 * 
 */
UCLASS()
class FACILITYBREACH_API UConeComponent : public UShapeComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shape)
	float ConeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shape)
	float ConeRadius;
	
};
