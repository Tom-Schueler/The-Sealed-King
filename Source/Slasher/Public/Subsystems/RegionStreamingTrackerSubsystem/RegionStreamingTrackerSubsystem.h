
#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "RegionStreamingTrackerSubsystem.generated.h"

class URegionStateSaveGame;

UCLASS()
class SLASHER_API URegionStreamingTrackerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:

    // Begin Subsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    // End Subsystem

private:

    void HandlePostLoadMapWithWorld(UWorld* World);
    void HandleLevelAdded(ULevel* InLevel, UWorld* InWorld);
    void HandleWorldBeginTearDown(UWorld* World);
    void HandlePreLevelRemoved(ULevel* InLevel, UWorld* InWorld);

    void SaveRegionState(ULevel* Level, UWorld* World);
    void LoadRegionState(ULevel* Level, UWorld* World);

    void OnRegionStateLoaded(const ULevel* InLevel, const URegionStateSaveGame* RegionState);

private:

    FDelegateHandle AddedHandle;
    FDelegateHandle PreRemovedHandle;
    FDelegateHandle PostLoadMapHandle;
    FDelegateHandle WorldBeginTearDownHandle;

    UPROPERTY(Transient)
    TMap<FName, FGuid> StatePersistendRegions;
};