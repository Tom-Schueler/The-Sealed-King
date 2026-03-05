
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerWorldStateSaveGame.generated.h"

UCLASS()
class SLASHER_API UPlayerWorldStateSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void DestroySaveGame();

public:

	UPROPERTY(SaveGame)
	TMap<FGuid, FString> RegionsSlotNames;

	UPROPERTY(SaveGame)
	FString PlayerSlotName;

	UPROPERTY(SaveGame)
	FString PlayerCollectionSlotName;
};