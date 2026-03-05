#pragma once

#include "CoreMinimal.h"
#include "CombatActionDirection.generated.h"

UENUM(BlueprintType)
enum class ECombatActionDirection : uint8
{
	Zero UMETA(DisplayName = "Zero/Default"),
	Forward UMETA(DisplayName = "Forward"),
	Right UMETA(DisplayName = "Right"),
	Back UMETA(DisplayName = "Back"),
	Left UMETA(DisplayName = "Left")
};