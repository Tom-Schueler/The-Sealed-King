
#pragma once

#include "CoreMinimal.h"
#include "WorldIdentifyerPair.generated.h"

class UWorld;

USTRUCT()
struct FWorldIdentifyerPair
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Region;

	UPROPERTY(EditDefaultsOnly)
	FGuid Guid;
};