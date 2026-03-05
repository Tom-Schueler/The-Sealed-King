
#include "SaveGames/RegionStateSaveGame.h"
#include "SaveGameProxyArchive.h"
#include "Kismet/GameplayStatics.h"

void URegionStateSaveGame::SetObject(const FGuid& Key, USaveGame* InObject)
{
	if (!Key.IsValid() || !InObject)
	{ return; }

	FSavedObjectBlob Blob;
	Blob.Class = InObject->GetClass();
	
	FMemoryWriter MemoryWriter(Blob.Data, /*bIsPersistent*/ true);
	FSaveGameProxyArchive Ar(MemoryWriter);

	InObject->SerializeScriptProperties(Ar);

	SavedObjects.Add(Key, MoveTemp(Blob));
}

void URegionStateSaveGame::GetObject(const FGuid& Key, USaveGame*& OutObject) const
{
	if (!Key.IsValid())
	{ return; }

	OutObject = nullptr;

	const FSavedObjectBlob* Blob = SavedObjects.Find(Key);
	if (!Blob || !Blob->Class)
	{ return; }

	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(Blob->Class);
	if (!SaveGame)
	{ return; }

	FMemoryReader MemoryReader(Blob->Data, /*bIsPersistent*/ true);
	FSaveGameProxyArchive Ar(MemoryReader);

	SaveGame->SerializeScriptProperties(Ar);
	OutObject = SaveGame; 
}
