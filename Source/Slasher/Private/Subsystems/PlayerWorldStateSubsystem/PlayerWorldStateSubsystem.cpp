
#include "Subsystems/PlayerWorldStateSubsystem/PlayerWorldStateSubsystem.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SaveGame.h"
#include "SaveGames/PlayerWorldStateSaveGame.h"
#include "SaveGames/CharacterStateSaveGame.h"
#include "SaveGames/RegionStateSaveGame.h"
#include "Misc/Guid.h"
#include "SaveGames/CollectiblesSaveGame.h"

void UPlayerWorldStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnPostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPlayerWorldStateSubsystem::OnPostLoadMap);
}

void UPlayerWorldStateSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(OnPostLoadMapHandle);

	Super::Deinitialize();
}

void UPlayerWorldStateSubsystem::RegisterConditionMet(UMoviePlayerSubsystem* MoviePlayerSubsystem, const FName& InFunctionName)
{
	ConditionMetDelegate.BindUFunction(MoviePlayerSubsystem, InFunctionName);
}

void UPlayerWorldStateSubsystem::OpenMap(const TSoftObjectPtr<UWorld> Map)
{
	if (IsLoadingNewWorldState)
	{
		FDelegateHandle Handle = OnNewWorldStateLoaded.AddLambda([this, Map, Handle]() {
			OnNewWorldStateLoaded.Remove(Handle);
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Map);
		});

		return;
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Map);
}

void UPlayerWorldStateSubsystem::OpenCurrentMap()
{
	if (IsLoadingNewWorldState)
	{
		FDelegateHandle Handle = OnNewWorldStateLoaded.AddLambda([this, Handle]() {
			OnNewWorldStateLoaded.Remove(Handle);
			const FName LevelName(*UGameplayStatics::GetCurrentLevelName(this, true));
			UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		});

		return;
	}

	const FName LevelName(*UGameplayStatics::GetCurrentLevelName(this, true));
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void UPlayerWorldStateSubsystem::SetPlayerWorldState(const FString& SlotName, TSoftObjectPtr<UWorld> Map, FSetPlayerWorldStateFailedDelegate OnFail)
{
	if (Map.IsNull() || !UGameplayStatics::DoesSaveGameExist(SlotName, 0) || SlotName == CurrentWorldStateSlotName)
	{
		OnFail.ExecuteIfBound();
		return;
	}

	OnBeforePlayerWorldStateChange.Broadcast();

	IsLoadingNewWorldState = true;

	if (CurrentWorldState)
	{
		FAsyncSaveGameToSlotDelegate OutSaveDelegate;
		OutSaveDelegate.BindLambda([this, SlotName, Map, OnFail](const FString&, const int32, bool Success) {

			if (!Success)
			{
				IsLoadingNewWorldState = false;
				OnFail.ExecuteIfBound();
				return;
			}

			SetPlayerWorldStateInternal(SlotName, Map, OnFail);
		});
		UGameplayStatics::AsyncSaveGameToSlot(CurrentWorldState, CurrentWorldStateSlotName, 0, OutSaveDelegate);

		return;
	}

	SetPlayerWorldStateInternal(SlotName, Map, OnFail);
}

void UPlayerWorldStateSubsystem::SetRegionState(const FGuid& Key, URegionStateSaveGame* RegionState)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty() || IsLoadingNewWorldState)
	{ return; }

	FStateSavedDelegate OutSavedDelegate;
	OutSavedDelegate.BindLambda([](bool Success) {
		if (!Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Well fuck"));
		}
	});

	TMap<FGuid, FString>& RegionsSlotNames = CurrentWorldState->RegionsSlotNames;

	if (const FString* SlotName = RegionsSlotNames.Find(Key))
	{
		SaveState(*SlotName, RegionState, OutSavedDelegate);
		return;
	}

	const FString NewSlotName = FGuid::NewGuid().ToString();
	RegionsSlotNames.Add(Key, NewSlotName);
	SaveState(NewSlotName, RegionState, OutSavedDelegate);
	SaveState(CurrentWorldStateSlotName, CurrentWorldState, OutSavedDelegate);
}

void UPlayerWorldStateSubsystem::GetRegionState(const FGuid& Key, FRegionStateLoadedDelegate OnDataLoaded)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty())
	{ 
		OnDataLoaded.ExecuteIfBound(nullptr);
		return; 
	}

	if (IsLoadingNewWorldState)
	{
		FDelegateHandle Handle = OnNewWorldStateLoaded.AddLambda([this, Key, OnDataLoaded, Handle]() {
			OnNewWorldStateLoaded.Remove(Handle);
			GetRegionStateInternal(Key, OnDataLoaded);
		});

		return;
	}

	GetRegionStateInternal(Key, OnDataLoaded);
}

void UPlayerWorldStateSubsystem::SetCharacterState(UCharacterStateSaveGame* CharacterState)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty() || IsLoadingNewWorldState)
	{ return; }

	FStateSavedDelegate OutSavedDelegate;
	OutSavedDelegate.BindLambda([](bool Success) {
		if (!Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Well fuck"));
		}
	});

	if (!CurrentWorldState->PlayerSlotName.IsEmpty())
	{
		SaveState(CurrentWorldState->PlayerSlotName, CharacterState, OutSavedDelegate);
		return;
	}

	FString NewSlotName = FGuid::NewGuid().ToString();
	CurrentWorldState->PlayerSlotName = NewSlotName;
	SaveState(NewSlotName, CharacterState, OutSavedDelegate);
	SaveState(CurrentWorldStateSlotName, CurrentWorldState, OutSavedDelegate);
}

void UPlayerWorldStateSubsystem::GetCharacterState(FCharacterStateLoadedDelegate OnDataLoaded)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty())
	{
		OnDataLoaded.ExecuteIfBound(nullptr);
		return;
	}

	if (IsLoadingNewWorldState)
	{
		FDelegateHandle Handle = OnNewWorldStateLoaded.AddLambda([this, OnDataLoaded, Handle]() {
			OnNewWorldStateLoaded.Remove(Handle);
			GetCharacterStateInternal(OnDataLoaded);
		});

		return;
	}

	GetCharacterStateInternal(OnDataLoaded);
}

void UPlayerWorldStateSubsystem::SetPlayerCollection(UCollectiblesSaveGame* PlayerCollection)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty() || IsLoadingNewWorldState)
	{ return; }

	FStateSavedDelegate OutSavedDelegate;
	OutSavedDelegate.BindLambda([](bool Success) {
		if (!Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Well fuck"));
		}
	});

	if (!CurrentWorldState->PlayerCollectionSlotName.IsEmpty())
	{
		SaveState(CurrentWorldState->PlayerCollectionSlotName, PlayerCollection, OutSavedDelegate);
		return;
	}

	FString NewSlotName = FGuid::NewGuid().ToString();
	CurrentWorldState->PlayerCollectionSlotName = NewSlotName;
	SaveState(NewSlotName, PlayerCollection, OutSavedDelegate);
	SaveState(CurrentWorldStateSlotName, CurrentWorldState, OutSavedDelegate);
}

void UPlayerWorldStateSubsystem::GetPlayerCollection(FPlayerCollectionLoadedDelegate OnLoaded)
{
	if (!CurrentWorldState || CurrentWorldStateSlotName.IsEmpty())
	{
		OnLoaded.ExecuteIfBound(nullptr);
		return;
	}

	if (IsLoadingNewWorldState)
	{
		FDelegateHandle Handle = OnNewWorldStateLoaded.AddLambda([this, OnLoaded, Handle]() {
			OnNewWorldStateLoaded.Remove(Handle);
			GetPlayerCollectionInternal(OnLoaded);
		});

		return;
	}

	GetPlayerCollectionInternal(OnLoaded);
}

void UPlayerWorldStateSubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
	ConditionMetDelegate.ExecuteIfBound(this);
	ConditionMetDelegate.Unbind();

	if (IsLoadingNewWorldState)
	{
		IsLoadingNewWorldState = false;
		OnNewWorldStateLoaded.Broadcast();
	}
}

void UPlayerWorldStateSubsystem::LoadState(const FString& SlotName, FStateLoadedDelegate OnStateLoaded)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		OnStateLoaded.ExecuteIfBound(nullptr);
		return;
	}

	FAsyncLoadGameFromSlotDelegate OutLoadDelegate;
	OutLoadDelegate.BindLambda([OnStateLoaded](const FString&, const int32, USaveGame* LoadedSaveGame) { OnStateLoaded.ExecuteIfBound(LoadedSaveGame); });
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, OutLoadDelegate);
}

void UPlayerWorldStateSubsystem::SaveState(const FString& SlotName, USaveGame* State, FStateSavedDelegate OnStateSaved)
{
	FAsyncSaveGameToSlotDelegate OutSaveDelegate;
	OutSaveDelegate.BindLambda([OnStateSaved](const FString&, const int32, bool Success) { OnStateSaved.ExecuteIfBound(Success); });
	UGameplayStatics::AsyncSaveGameToSlot(State, SlotName, 0, OutSaveDelegate);
}

void UPlayerWorldStateSubsystem::GetCharacterStateInternal(FCharacterStateLoadedDelegate OnDataLoaded)
{
	FStateLoadedDelegate OutStateLoadedDelegate;
	OutStateLoadedDelegate.BindLambda([OnDataLoaded](USaveGame* CharacterState) { OnDataLoaded.ExecuteIfBound(Cast<UCharacterStateSaveGame>(CharacterState)); });
	LoadState(CurrentWorldState->PlayerSlotName, OutStateLoadedDelegate);
}

void UPlayerWorldStateSubsystem::GetRegionStateInternal(const FGuid& Key, FRegionStateLoadedDelegate OnDataLoaded)
{
	const FString* SlotName = CurrentWorldState->RegionsSlotNames.Find(Key);
	if (!SlotName)
	{
		OnDataLoaded.ExecuteIfBound(nullptr);
		return;
	}

	FStateLoadedDelegate OutStateLoadedDelegate;
	OutStateLoadedDelegate.BindLambda([OnDataLoaded](USaveGame* RegionState) { OnDataLoaded.ExecuteIfBound(Cast<URegionStateSaveGame>(RegionState)); });
	LoadState(*SlotName, OutStateLoadedDelegate);
}

void UPlayerWorldStateSubsystem::SetPlayerWorldStateInternal(const FString& SlotName, TSoftObjectPtr<UWorld> Map, FSetPlayerWorldStateFailedDelegate OnFail)
{
	FStateLoadedDelegate OutLoadDelegate;
	OutLoadDelegate.BindLambda([this, SlotName, Map, OnFail](USaveGame* WorldState) {
		if (!WorldState)
		{
			IsLoadingNewWorldState = false;
			OnFail.ExecuteIfBound();
			return;
		}

		CurrentWorldState = Cast<UPlayerWorldStateSaveGame>(WorldState);
		CurrentWorldStateSlotName = SlotName;

		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Map);
	});
	LoadState(SlotName, OutLoadDelegate);
}

void UPlayerWorldStateSubsystem::GetPlayerCollectionInternal(FPlayerCollectionLoadedDelegate OnLoaded)
{
	FStateLoadedDelegate OutStateLoadedDelegate;
	OutStateLoadedDelegate.BindLambda([OnLoaded](USaveGame* PlayerCollection) { OnLoaded.ExecuteIfBound(Cast<UCollectiblesSaveGame>(PlayerCollection)); });
	LoadState(CurrentWorldState->PlayerCollectionSlotName, OutStateLoadedDelegate);
}
