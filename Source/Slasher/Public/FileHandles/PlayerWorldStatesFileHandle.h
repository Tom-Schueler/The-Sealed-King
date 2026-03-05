
#pragma once

#include "CoreMinimal.h"
#include "FileHandles/_Common/FileHandle.h"
#include "PlayerWorldStatesFileHandle.generated.h"

class UPlayerWorldStatesSaveGame;

DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayerWorldStatesLoadedDelegate, UPlayerWorldStatesSaveGame*, SaveGame);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayerWorldStatesSavedDelegate, bool, Success);

UCLASS(BlueprintType)
class SLASHER_API UPlayerWorldStatesFileHandle : public UFileHandle
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Load(FPlayerWorldStatesLoadedDelegate OnLoaded);

	UFUNCTION(BlueprintCallable)
	void Save(UPlayerWorldStatesSaveGame* SaveGame, FPlayerWorldStatesSavedDelegate OnSaved);

protected:

	virtual const FString& GetSlotName() const override
	{
		static const FString SlotName(TEXT("PlayerSaveGames"));
		return SlotName;
	}
};