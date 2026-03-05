
#include "SaveGames/PlayerWorldStatesSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/PlayerWorldStateSaveGame.h"

bool UPlayerWorldStatesSaveGame::CreateNewSaveGame(const FString& SlotName)
{
	if (SlotNames.Contains(SlotName))
	{ return false; }

	USaveGame* EmptyState = UGameplayStatics::CreateSaveGameObject(UPlayerWorldStateSaveGame::StaticClass());
	UGameplayStatics::SaveGameToSlot(EmptyState, SlotName, 0);

	LastSlotName = SlotName;
	SlotNames.Add(SlotName);
	return true;
}

void UPlayerWorldStatesSaveGame::CreateNewSaveGameOrOverwrite(const FString& SlotName)
{
	USaveGame* EmptyState = UGameplayStatics::CreateSaveGameObject(UPlayerWorldStateSaveGame::StaticClass());
	UGameplayStatics::SaveGameToSlot(EmptyState, SlotName, 0);

	LastSlotName = SlotName;

	if (!SlotNames.Contains(SlotName))
	{
		SlotNames.Add(SlotName);
	}
}

void UPlayerWorldStatesSaveGame::RemoveAllButLastSlotNames()
{
	for (const FString& SlotName : SlotNames)
	{
		if (SlotName == LastSlotName)
		{ continue; }

		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
		UPlayerWorldStateSaveGame* PlayerWorldState = Cast<UPlayerWorldStateSaveGame>(SaveGame);

		if (PlayerWorldState)
		{
			PlayerWorldState->DestroySaveGame();
		}

		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}
}
