
#include "GameInstance/SlasherGameInstance.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerSubsystem.h"
#include "FileHandles/SettingsFileHandle.h"
#include "SaveGames/SettingsSaveGame.h"

void USlasherGameInstance::Init()
{
	Super::Init();

	IsInitialized = false;

	SettingsFileHandle = NewObject<USettingsFileHandle>();

	FSettingsLoadedDelegate OnLoadedDelegate;
	OnLoadedDelegate.BindUFunction(this, FName(TEXT("OnSettingsLoaded")));
	SettingsFileHandle->Load(OnLoadedDelegate);
}

void USlasherGameInstance::RegisterConditionMet(UMoviePlayerSubsystem* MoviePlayerSubsystem, const FName& InFunctionName)
{
	ConditionMetDelegate.BindUFunction(MoviePlayerSubsystem, InFunctionName);

	if (IsInitialized)
	{
		ConditionMetDelegate.ExecuteIfBound(this);
		ConditionMetDelegate.Unbind();
	}
}

void USlasherGameInstance::OnSettingsLoaded(USettingsSaveGame* SaveGame)
{
	// TODO: Apply settings or defualt settingsS

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Settings Loaded. Ready to apply!"));
	UE_LOG(LogTemp, Warning, TEXT("OnSettingsLoaded"));

	IsInitialized = true;
	ConditionMetDelegate.ExecuteIfBound(this);
	ConditionMetDelegate.Unbind();
}
