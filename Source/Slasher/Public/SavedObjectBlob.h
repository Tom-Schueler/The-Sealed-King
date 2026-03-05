
#pragma once

#include "SavedObjectBlob.generated.h"

class USaveGame;

USTRUCT(BlueprintType)
struct FSavedObjectBlob
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    TSubclassOf<USaveGame> Class;

    UPROPERTY(SaveGame)
    TArray<uint8> Data;
};