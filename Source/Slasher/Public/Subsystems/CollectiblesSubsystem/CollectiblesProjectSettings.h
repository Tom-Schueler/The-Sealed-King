
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CollectiblesProjectSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Collectibles"))
class SLASHER_API UCollectiblesProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	TSet<FName> Collectables;
};