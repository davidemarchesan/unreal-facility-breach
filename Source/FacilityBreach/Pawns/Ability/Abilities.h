#pragma once

#include "CoreMinimal.h"
#include "Abilities.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	ABILITY_Dash,
	ABILITY_Invisibility
};

USTRUCT(BlueprintType)
struct FAbilityTableRow : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EAbilityType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 MaxCharges = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float Cooldown = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bIsRechargeable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FName Icon = "";
	
};
