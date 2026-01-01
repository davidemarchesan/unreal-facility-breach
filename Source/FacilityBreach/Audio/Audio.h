#pragma once

#include "CoreMinimal.h"
#include "Audio.generated.h"

USTRUCT(BlueprintType)
struct FGameAudioTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TObjectPtr<USoundBase> Sound;
	
};
