#pragma once

#include "CoreMinimal.h"
#include "Stencils.generated.h"

UENUM(BlueprintType)
enum class EStencilType : uint8
{
	STENCIL_Custom = 0,
	STENCIL_Outline = 1,
	STENCIL_Glow = 2,
	STENCIL_OutlineGlow = 3
};
