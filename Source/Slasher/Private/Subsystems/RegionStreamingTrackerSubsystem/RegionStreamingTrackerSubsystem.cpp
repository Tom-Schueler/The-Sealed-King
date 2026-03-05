
#include "Subsystems/RegionStreamingTrackerSubsystem/RegionStreamingTrackerSubsystem.h"
#include "Subsystems/RegionStreamingTrackerSubsystem/RegionStreamingTrackerSettings.h"
#include "Subsystems/PlayerWorldStateSubsystem/PlayerWorldStateSubsystem.h"
#include "Engine/World.h"
#include "Engine/Level.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGames/RegionStateSaveGame.h"
#include "PersistentState.h"

bool static TryGetPlayerWorldStateSubsystem(UWorld* World, UPlayerWorldStateSubsystem*& OutSubsystem)
{
    OutSubsystem = nullptr;

    if (!World)
    {
        return false;
    }

    UGameInstance* GameInstance = World->GetGameInstance();
    if (!GameInstance)
    {
        return false;
    }

    OutSubsystem = GameInstance->GetSubsystem<UPlayerWorldStateSubsystem>();
    return OutSubsystem != nullptr;
}

const FName static GetPackageFName(ULevel* Level)
{
    if (!Level)
    {
        return FName();
    }

    const FString PackageName = UWorld::RemovePIEPrefix(Level->GetPackage()->GetName());
    return FName(*PackageName);
}

void URegionStreamingTrackerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (const URegionStreamingTrackerSettings* Settings = GetDefault<URegionStreamingTrackerSettings>())
    {
        for (const FWorldIdentifyerPair& pair : Settings->StatePersistendRegions)
        {
            StatePersistendRegions.Add(pair.Region.GetLongPackageFName(), pair.Guid);
        }
    }

    PostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &URegionStreamingTrackerSubsystem::HandlePostLoadMapWithWorld);
    WorldBeginTearDownHandle = FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &URegionStreamingTrackerSubsystem::HandleWorldBeginTearDown);
    AddedHandle = FWorldDelegates::LevelAddedToWorld.AddUObject(this, &URegionStreamingTrackerSubsystem::HandleLevelAdded);
    PreRemovedHandle = FWorldDelegates::PreLevelRemovedFromWorld.AddUObject(this, &URegionStreamingTrackerSubsystem::HandlePreLevelRemoved);
}

void URegionStreamingTrackerSubsystem::Deinitialize()
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadMapHandle);
    FWorldDelegates::OnWorldBeginTearDown.Remove(WorldBeginTearDownHandle);
    FWorldDelegates::LevelAddedToWorld.Remove(AddedHandle);
    FWorldDelegates::PreLevelRemovedFromWorld.Remove(PreRemovedHandle);

    Super::Deinitialize();
}

bool URegionStreamingTrackerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    const UWorld* World = Cast<UWorld>(Outer);
    return World && World->IsGameWorld();
}

void URegionStreamingTrackerSubsystem::HandlePostLoadMapWithWorld(UWorld* World)
{
    if (!World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    ULevel* PersistentLevel = World->PersistentLevel;
    LoadRegionState(PersistentLevel, World);
}

void URegionStreamingTrackerSubsystem::HandleLevelAdded(ULevel* Level, UWorld* World)
{
    if (!Level || !World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    LoadRegionState(Level, World);
}

void URegionStreamingTrackerSubsystem::HandleWorldBeginTearDown(UWorld* World)
{
    if (!World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    for (ULevel* Level : World->GetLevels())
    {
        SaveRegionState(Level, World);
    }
}

void URegionStreamingTrackerSubsystem::HandlePreLevelRemoved(ULevel* Level, UWorld* World)
{
    if (!Level || !World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    SaveRegionState(Level, World);
}

void URegionStreamingTrackerSubsystem::SaveRegionState(ULevel* Level, UWorld* World)
{
    if (!Level || !World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    const FName PackageFName = GetPackageFName(Level);
    if (PackageFName.IsNone())
    { return; }

    UPlayerWorldStateSubsystem* OutPlayerWorldStateSubsystem = nullptr;
    if (!TryGetPlayerWorldStateSubsystem(World, OutPlayerWorldStateSubsystem))
    { return; }

    FGuid* RegionGuid = StatePersistendRegions.Find(PackageFName);
    if (!RegionGuid)
    { return; }

    URegionStateSaveGame* RegionState = Cast<URegionStateSaveGame>(UGameplayStatics::CreateSaveGameObject(URegionStateSaveGame::StaticClass()));
    for (const TObjectPtr<AActor> Actor : Level->Actors)
    {
        if (!IsValid(Actor)) continue;
        if (!Actor->Implements<UPersistentState>())
        { continue; }

        const FGuid ObjectGuid = IPersistentState::Execute_GetGuid(Actor);
        if (!ObjectGuid.IsValid())
        { continue; }

        USaveGame* State = IPersistentState::Execute_GetData(Actor);
        RegionState->SetObject(ObjectGuid, State);
    }

    OutPlayerWorldStateSubsystem->SetRegionState(*RegionGuid, RegionState);
}

void URegionStreamingTrackerSubsystem::LoadRegionState(ULevel* Level, UWorld* World)
{
    if (!Level || !World || !World->IsGameWorld() || World != GetWorld())
    { return; }

    const FName PackageFName = GetPackageFName(Level);
    if (PackageFName.IsNone())
    { return; }

    UPlayerWorldStateSubsystem* OutPlayerWorldStateSubsystem = nullptr;
    if (!TryGetPlayerWorldStateSubsystem(World, OutPlayerWorldStateSubsystem))
    { return; }

    FGuid* Guid = StatePersistendRegions.Find(PackageFName);
    if (!Guid)
    { return; }

    TWeakObjectPtr<URegionStreamingTrackerSubsystem> WeakThis(this);
    TWeakObjectPtr<ULevel> WeakLevel(Level);

    FRegionStateLoadedDelegate OutLoadedDelegate;
    OutLoadedDelegate.BindLambda([WeakThis, WeakLevel](URegionStateSaveGame* RegionState) {
        if (WeakThis.IsValid() && WeakLevel.IsValid() && RegionState)
        { 
            WeakThis->OnRegionStateLoaded(WeakLevel.Get(), RegionState);
        }
    });
    OutPlayerWorldStateSubsystem->GetRegionState(*Guid, OutLoadedDelegate);
}

void URegionStreamingTrackerSubsystem::OnRegionStateLoaded(const ULevel* InLevel, const URegionStateSaveGame* RegionState)
{
    if (!RegionState || !InLevel)
    { return; }

    for (AActor* Actor : InLevel->Actors)
    {
        if (!IsValid(Actor) || !Actor->Implements<UPersistentState>())
        { continue; }

        const FGuid ObjectGuid = IPersistentState::Execute_GetGuid(Actor);
        if (!ObjectGuid.IsValid())
        { continue; }

        USaveGame* OutState = nullptr;
        RegionState->GetObject(ObjectGuid, OutState);
        if (!OutState)
        { continue; }

        IPersistentState::Execute_SetData(Actor, OutState);
    }
}
