
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerStopConditionOwner.h"
#include "PlayerWorldStateSubsystem.generated.h"

class UPlayerWorldStateSaveGame;
class UCharacterStateSaveGame;
class USaveGame;
class URegionStateSaveGame;
class UCollectiblesSaveGame;

DECLARE_DYNAMIC_DELEGATE_OneParam(FCharacterStateLoadedDelegate, UCharacterStateSaveGame*, Data);
DECLARE_DELEGATE_OneParam(FRegionStateLoadedDelegate, URegionStateSaveGame*);
DECLARE_DELEGATE_OneParam(FPlayerCollectionLoadedDelegate, UCollectiblesSaveGame*)
DECLARE_DYNAMIC_DELEGATE(FSetPlayerWorldStateFailedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeforePlayerWorldStateChangeDelegate);

DECLARE_MULTICAST_DELEGATE(FNewWorldStateLoadedDelegate);
DECLARE_DELEGATE_OneParam(FStateLoadedDelegate, USaveGame*);
DECLARE_DELEGATE_OneParam(FStateSavedDelegate, bool);

UCLASS()
class SLASHER_API UPlayerWorldStateSubsystem : public UGameInstanceSubsystem, public IMoviePlayerStopConditionOwner
{
	GENERATED_BODY()

public:

	// Begin Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End Subsystem

	// Begin IMoviePlayerStopConditionOwner
	virtual void RegisterConditionMet(UMoviePlayerSubsystem* MoviePlayerSubsystem, const FName& InFunctionName) override;
	// End IMoviePlayerStopConditionOwner

	UFUNCTION(BlueprintCallable)
	void OpenMap(const TSoftObjectPtr<UWorld> Map);

	UFUNCTION(BlueprintCallable)
	void OpenCurrentMap();

	UFUNCTION(BlueprintCallable)
	void SetPlayerWorldState(const FString& SlotName, TSoftObjectPtr<UWorld> Map, FSetPlayerWorldStateFailedDelegate OnFail);

	void SetRegionState(const FGuid& Key, URegionStateSaveGame* RegionState);
	void GetRegionState(const FGuid& Key, FRegionStateLoadedDelegate OnDataLoaded);

	UFUNCTION(BlueprintCallable)
	void SetCharacterState(UCharacterStateSaveGame* CharacterState);

	UFUNCTION(BlueprintCallable)
	void GetCharacterState(FCharacterStateLoadedDelegate OnDataLoaded);


	void SetPlayerCollection(UCollectiblesSaveGame* PlayerCollection);
	void GetPlayerCollection(FPlayerCollectionLoadedDelegate OnLoaded);

private:

	void OnPostLoadMap(UWorld* LoadedWorld);
	void LoadState(const FString& SlotName, FStateLoadedDelegate OnStateLoaded);
	void SaveState(const FString& SlotName, USaveGame* State, FStateSavedDelegate OnStateSaved);
	void GetCharacterStateInternal(FCharacterStateLoadedDelegate OnDataLoaded);
	void GetRegionStateInternal(const FGuid& Key, FRegionStateLoadedDelegate OnDataLoaded);
	void SetPlayerWorldStateInternal(const FString& SlotName, TSoftObjectPtr<UWorld> Map, FSetPlayerWorldStateFailedDelegate OnFail);
	void GetPlayerCollectionInternal(FPlayerCollectionLoadedDelegate OnLoaded);

public:

	UPROPERTY(BlueprintAssignable)
	FBeforePlayerWorldStateChangeDelegate OnBeforePlayerWorldStateChange;

	FNewWorldStateLoadedDelegate OnNewWorldStateLoaded;

private:

	UPROPERTY(Transient)
	UPlayerWorldStateSaveGame* CurrentWorldState;

	UPROPERTY(Transient)
	FString CurrentWorldStateSlotName;

	FConditionMetDelegate ConditionMetDelegate;
	FDelegateHandle OnPostLoadMapHandle;
	
	bool IsLoadingNewWorldState = false;
};