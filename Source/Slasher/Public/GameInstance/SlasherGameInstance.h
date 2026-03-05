
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerStopConditionOwner.h"
#include "SlasherGameInstance.generated.h"

class USettingsSaveGame;
class USettingsFileHandle;

UCLASS()
class SLASHER_API USlasherGameInstance final : public UGameInstance, public IMoviePlayerStopConditionOwner
{
	GENERATED_BODY()

public:

	// Begin UGameInstance
	virtual void Init() override;
	// Begin UGameInstance

	// Begin IMoviePlayerStopConditionOwner
	virtual void RegisterConditionMet(UMoviePlayerSubsystem* MoviePlayerSubsystem, const FName& InFunctionName) override;
	// End IMoviePlayerStopConditionOwner

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsInitialized() const { return IsInitialized; }

private:

	UFUNCTION()
	void OnSettingsLoaded(USettingsSaveGame* SaveGame);

private:

	bool IsInitialized = false;

	UPROPERTY()
	FConditionMetDelegate ConditionMetDelegate;

	UPROPERTY()
	USettingsFileHandle* SettingsFileHandle = nullptr;
};