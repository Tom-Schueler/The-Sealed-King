
#include "FileHandles/_Common/FileHandle.h"
#include "Kismet/GameplayStatics.h"

bool UFileHandle::GetIsProcessing() const
{
    return IsProcessing;
}

void UFileHandle::InternalLoad(FLoadFinishedDelegate LoadedDelegate)
{
    if (IsProcessing)
    {
        LoadedDelegate.ExecuteIfBound(nullptr);
        return;
    }

    IsProcessing = true;
    FAsyncLoadGameFromSlotDelegate OutDelegate;

    OutDelegate.BindLambda([this, LoadedDelegate](const FString&, const int32, USaveGame* SaveGame) {

        IsProcessing = false;
        LoadedDelegate.ExecuteIfBound(SaveGame);
    });

    UGameplayStatics::AsyncLoadGameFromSlot(GetSlotName(), 0, OutDelegate);
}

void UFileHandle::InternalSave(USaveGame* SaveGame, FSaveFinishedDelegate SavedDelegate)
{
    if (IsProcessing)
    {
        SavedDelegate.ExecuteIfBound(false);
        return;
    }

    IsProcessing = true;
    FAsyncSaveGameToSlotDelegate OutDelegate;

    OutDelegate.BindLambda([this, SavedDelegate](const FString&, const int32, bool Success) {

        IsProcessing = false;
        SavedDelegate.ExecuteIfBound(Success);
    });

    UGameplayStatics::AsyncSaveGameToSlot(SaveGame, GetSlotName(), 0, OutDelegate);
}
