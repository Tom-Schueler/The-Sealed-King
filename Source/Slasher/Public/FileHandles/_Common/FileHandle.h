#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FileHandle.generated.h"

class USaveGame;

DECLARE_DELEGATE_OneParam(FLoadFinishedDelegate, USaveGame*);
DECLARE_DELEGATE_OneParam(FSaveFinishedDelegate, bool);


UCLASS(Abstract)
class SLASHER_API UFileHandle : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsProcessing() const;

protected:

	void InternalLoad(FLoadFinishedDelegate LoadedDelegate);
	void InternalSave(USaveGame* SaveGame, FSaveFinishedDelegate SavedDelegate);

	virtual const FString& GetSlotName() const PURE_VIRTUAL(UFileHandle::GetSlotName, static const FString Dummy(TEXT("INVALID")); return Dummy;);

private:

	bool IsProcessing = false;
};