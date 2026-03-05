
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CollectiblesSaveGame.generated.h"

UCLASS()
class SLASHER_API UCollectiblesSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FName, bool> Collectibles;

};