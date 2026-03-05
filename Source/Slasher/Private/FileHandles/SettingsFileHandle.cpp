
#include "FileHandles/SettingsFileHandle.h"
#include "SaveGames/SettingsSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USettingsFileHandle::Load(FSettingsLoadedDelegate OnLoaded)
{
	FLoadFinishedDelegate OutDelegate;
	OutDelegate.BindLambda([OnLoaded](USaveGame* SaveGame) {
		USettingsSaveGame* SettingsSaveGame = Cast<USettingsSaveGame>(SaveGame);
		if (!SettingsSaveGame)
		{
			SettingsSaveGame = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass()));
		}

		OnLoaded.ExecuteIfBound(SettingsSaveGame);
	});
	InternalLoad(OutDelegate);
}

void USettingsFileHandle::Save(USettingsSaveGame* SaveGame, FSettingsSavedDelegate OnSaved)
{
	FSaveFinishedDelegate OutDelegate;
	OutDelegate.BindLambda([OnSaved](bool Success) { OnSaved.ExecuteIfBound(Success); });
	InternalSave(SaveGame, OutDelegate);
}
