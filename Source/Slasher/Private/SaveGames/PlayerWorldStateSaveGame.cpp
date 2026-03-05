
#include "SaveGames/PlayerWorldStateSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UPlayerWorldStateSaveGame::DestroySaveGame()
{
	for (const auto& Slot : RegionsSlotNames)
	{
		UGameplayStatics::DeleteGameInSlot(Slot.Value, 0);
	}
	RegionsSlotNames.Empty();

	UGameplayStatics::DeleteGameInSlot(PlayerCollectionSlotName, 0);
	PlayerCollectionSlotName = FString();

	UGameplayStatics::DeleteGameInSlot(PlayerSlotName, 0);
	PlayerSlotName = FString();
}
