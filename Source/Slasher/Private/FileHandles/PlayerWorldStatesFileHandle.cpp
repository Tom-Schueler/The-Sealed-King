
#include "FileHandles/PlayerWorldStatesFileHandle.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/PlayerWorldStatesSaveGame.h"

void UPlayerWorldStatesFileHandle::Load(FPlayerWorldStatesLoadedDelegate OnLoaded)
{
	FLoadFinishedDelegate OutDelegate;
	OutDelegate.BindLambda([OnLoaded](USaveGame* SaveGame) {
		UPlayerWorldStatesSaveGame* PlayerWorldStatesSaveGame = Cast<UPlayerWorldStatesSaveGame>(SaveGame);
		if (!PlayerWorldStatesSaveGame)
		{
			PlayerWorldStatesSaveGame = Cast<UPlayerWorldStatesSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerWorldStatesSaveGame::StaticClass()));
		}

		OnLoaded.ExecuteIfBound(PlayerWorldStatesSaveGame);
	});
	InternalLoad(OutDelegate);
}

void UPlayerWorldStatesFileHandle::Save(UPlayerWorldStatesSaveGame* SaveGame, FPlayerWorldStatesSavedDelegate OnSaved)
{
	FSaveFinishedDelegate OutDelegate;
	OutDelegate.BindLambda([OnSaved](bool Success) { OnSaved.ExecuteIfBound(Success); });
	InternalSave(SaveGame, OutDelegate);
}
