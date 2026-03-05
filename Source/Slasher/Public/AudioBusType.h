
#pragma once

#include "CoreMinimal.h"
#include "AudioBusType.generated.h"

UENUM(BlueprintType)
enum class EAudioBusType : uint8
{
	Master UMETA(DisplayName = "Master"),
	Music UMETA(DisplayName = "Music"),
	SFX UMETA(DisplayName = "SFX"),
	Ambient UMETA(DisplayName = "Ambient"),
	UserInterface UMETA(DisplayName = "UserInterface"),
	Voice UMETA(DisplayName = "Voice")
};