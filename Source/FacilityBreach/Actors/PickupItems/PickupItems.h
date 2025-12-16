#pragma once

#include "CoreMinimal.h"
#include "PickupItems.generated.h"

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Item")
	FString Name;
	
};
