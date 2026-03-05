#pragma once

#include "CoreMinimal.h"
#include "CombatAction.generated.h"

UENUM(BlueprintType)
enum class ECombatAction : uint8
{
	Dodge UMETA(DisplayName = "Dodge"),
	LightAttack UMETA(DisplayName = "LightAttack"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
	Execute UMETA(DisplayName = "Execute"),
	Block UMETA(DisplayName = "Block")
};