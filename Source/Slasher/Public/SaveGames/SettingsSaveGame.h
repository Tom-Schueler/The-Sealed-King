
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AudioBusType.h"
#include "SettingsSaveGame.generated.h"

UCLASS(BlueprintType)
class SLASHER_API USettingsSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:

	// Sound Settigs Start
	UPROPERTY(BlueprintReadWrite, SaveGame)
	TMap<EAudioBusType, float> BusVolumes;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TMap<EAudioBusType, bool> BusMuteStates;
	// Sound Settings End

};