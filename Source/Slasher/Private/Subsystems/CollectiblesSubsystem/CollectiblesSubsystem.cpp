
#include "Subsystems/CollectiblesSubsystem/CollectiblesSubsystem.h"
#include "Subsystems/PlayerWorldStateSubsystem/PlayerWorldStateSubsystem.h"
#include "Subsystems/CollectiblesSubsystem/CollectiblesProjectSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/CollectiblesSaveGame.h"

void UCollectiblesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UPlayerWorldStateSubsystem* PWS = Collection.InitializeDependency<UPlayerWorldStateSubsystem>();

	const UCollectiblesProjectSettings* Settings = GetDefault<UCollectiblesProjectSettings>();
	if (Settings)
	{
		for (const FName& Collectible : Settings->Collectables)
		{
			Collectibles.Add(Collectible, false);
		}
	}

	LoadPlayerCollectionHandle = PWS->OnNewWorldStateLoaded.AddUObject(this, &UCollectiblesSubsystem::HandleLoadPlayerCollection);
}

void UCollectiblesSubsystem::Deinitialize()
{
	if (UPlayerWorldStateSubsystem* PWS = GetGameInstance()->GetSubsystem<UPlayerWorldStateSubsystem>())
	{
		PWS->OnNewWorldStateLoaded.Remove(LoadPlayerCollectionHandle);
	}
	
	Super::Deinitialize();
}

bool UCollectiblesSubsystem::SetCollected(const FName& ColectableName)
{
	if (!Collectibles.Contains(ColectableName))
	{ return false; }

	Collectibles[ColectableName] = true;

	if (UPlayerWorldStateSubsystem* PWS = GetGameInstance()->GetSubsystem<UPlayerWorldStateSubsystem>())
	{
		UCollectiblesSaveGame* SaveGame = Cast<UCollectiblesSaveGame>(UGameplayStatics::CreateSaveGameObject(UCollectiblesSaveGame::StaticClass()));
		SaveGame->Collectibles.Append(Collectibles);
		PWS->SetPlayerCollection(SaveGame);

		UE_LOG(LogTemp, Warning, TEXT("Collection should be saved"));
	}

	return true;
}

void UCollectiblesSubsystem::HandleLoadPlayerCollection()
{
	if (UPlayerWorldStateSubsystem* PWS = GetGameInstance()->GetSubsystem<UPlayerWorldStateSubsystem>())
	{
		FPlayerCollectionLoadedDelegate OutLoaded;
		OutLoaded.BindUObject(this, &UCollectiblesSubsystem::HandlePlayerCollectionLoaded);
		PWS->GetPlayerCollection(OutLoaded);
	}
}

void UCollectiblesSubsystem::HandlePlayerCollectionLoaded(UCollectiblesSaveGame* PlayerCollection)
{
	if (PlayerCollection)
	{
		for (const auto& Collectible : PlayerCollection->Collectibles)
		{
			UE_LOG(LogTemp, Warning, TEXT("item in collection"));
			if (!Collectibles.Contains(Collectible.Key))
			{ continue; }

			Collectibles[Collectible.Key] = Collectible.Value;
		}

		UE_LOG(LogTemp, Warning, TEXT("Collection should be loaded"));
	}
	else
	{
		TArray<FName> OutKeys;
		Collectibles.GetKeys(OutKeys);
		for (const FName& CollectibleName : OutKeys)
		{
			Collectibles[CollectibleName] = false;
		}
	}
}
