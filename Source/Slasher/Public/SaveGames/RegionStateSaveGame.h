
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedObjectBlob.h"
#include "RegionStateSaveGame.generated.h"

UCLASS(BlueprintType)
class SLASHER_API URegionStateSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:

	void SetObject(const FGuid& Key, USaveGame* InObject);
	void GetObject(const FGuid& Key, USaveGame*& OutObject) const;

private:

	UPROPERTY(SaveGame)
	TMap<FGuid, FSavedObjectBlob> SavedObjects; 
};