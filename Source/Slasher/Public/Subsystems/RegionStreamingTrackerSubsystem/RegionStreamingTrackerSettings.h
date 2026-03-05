
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Subsystems/RegionStreamingTrackerSubsystem/WorldIdentifyerPair.h"
#include "RegionStreamingTrackerSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "RegionStreamingTracker"))
class SLASHER_API URegionStreamingTrackerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FWorldIdentifyerPair> StatePersistendRegions;
};