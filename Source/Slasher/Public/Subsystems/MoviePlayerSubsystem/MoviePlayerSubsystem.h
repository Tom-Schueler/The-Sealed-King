// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MoviePlayer.h"
#include "MoviePlayerSubsystem.generated.h"

class UMoviePlayerStopConditionOwner;

/**
 * 
 */
UCLASS(NotBlueprintable)
class SLASHER_API UMoviePlayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Begin Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End Subsystem

	UFUNCTION(BlueprintCallable)
	bool StartLoadingScreen(const TSubclassOf<UUserWidget> loadingScreenWidget, const TArray<FString>& moviePaths);

	UFUNCTION(BlueprintCallable)
	bool StartDefaultLoadingScreen();

	UFUNCTION(BlueprintCallable)
	bool AddFinishCondition(const TScriptInterface<UMoviePlayerStopConditionOwner>& ConditionOwner);

	UFUNCTION(BlueprintCallable)
	bool AddFinishConditions(const TArray<TScriptInterface<UMoviePlayerStopConditionOwner>>& ConditionOwners);

private:

	UFUNCTION()
	void OnFinishConditionMet(const UObject* ConditionOwner);

	bool AddFinishConditionRaw(const TScriptInterface<UMoviePlayerStopConditionOwner>& ConditionOwner);
	bool GetMoviePlayerHelper(IGameMoviePlayer*& OutMoviePlayer);
	void EndLoadingScreen();

	void HandlePostLoadMap(UWorld* LoadedWorld);

	void LoadingScreenTick(float DeltaTime);

	// Begin Default Settings
	UPROPERTY(Transient)
	TSubclassOf<UUserWidget> DefaultLoadingScreenWidget;

	UPROPERTY(Transient)
	TArray<FString> DefaultMoviePaths;

	float MinimumLoadingScreenDisplayTime;
	// End Default Settings

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> LoadingWidgetInstance;
	TSharedPtr<SWidget> LoadingSlateWidget;

	UPROPERTY(Transient)
	TSet<UObject*> CachedConditionOwners;

	double LoadingScreenStartTime;
	bool LoadFinished;
	FDelegateHandle Handle;
};
