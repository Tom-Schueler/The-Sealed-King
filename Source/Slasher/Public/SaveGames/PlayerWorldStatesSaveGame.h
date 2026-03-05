
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerWorldStatesSaveGame.generated.h"

UCLASS(BlueprintType)
class SLASHER_API UPlayerWorldStatesSaveGame final : public USaveGame
{
	GENERATED_BODY()

public:

	/*  
	* @return Returns false if Slot exists
	*/
	UFUNCTION(BlueprintCallable)
	bool CreateNewSaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void CreateNewSaveGameOrOverwrite(const FString& SlotName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FString& GetLastSlotName() const { return LastSlotName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FString& GetSlotName(const int Index)
	{
		const FString& SlotName = SlotNames[Index];
		LastSlotName = SlotName;
		return SlotName;
	}

	UFUNCTION(BlueprintCallable)
	void RemoveAllButLastSlotNames();

	/*
	* Note: Use this to view the Slot Names or display them. Dont use this to select the SlotToLoad a game from. For that use "GetSlotName()".
	* 
	* @return Returns the SlotNames saved in this object.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TArray<FString>& GetSlotNames() const { return SlotNames; }

private:

	UPROPERTY(SaveGame)
	FString LastSlotName;

	UPROPERTY(SaveGame)
	TArray<FString> SlotNames;
};