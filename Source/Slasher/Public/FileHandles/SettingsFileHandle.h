
#pragma once

#include "CoreMinimal.h"
#include "FileHandles/_Common/FileHandle.h"
#include "SettingsFileHandle.generated.h"

class USettingsSaveGame;

DECLARE_DYNAMIC_DELEGATE_OneParam(FSettingsLoadedDelegate, USettingsSaveGame*, SaveGame);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSettingsSavedDelegate, bool, Success);

UCLASS(BlueprintType)
class SLASHER_API USettingsFileHandle final : public UFileHandle
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Load(FSettingsLoadedDelegate OnLoaded);


	UFUNCTION(BlueprintCallable)
	void Save(USettingsSaveGame* SaveGame, FSettingsSavedDelegate OnSaved);

protected:

	virtual const FString& GetSlotName() const override
	{
		static const FString SlotName(TEXT("Settings"));
		return SlotName;
	}
};