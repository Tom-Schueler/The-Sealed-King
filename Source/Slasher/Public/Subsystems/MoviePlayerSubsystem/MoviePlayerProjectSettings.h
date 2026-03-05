
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MoviePlayerProjectSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "MoviePlayer Settings"))
class SLASHER_API UMoviePlayerProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

private:

#if WITH_EDITOR
	void SanitizeMoviePath(FString& InOutPath);
#endif

public:
	UMoviePlayerProjectSettings();

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UUserWidget> DefaultLoadingScreenWidget;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FFilePath> DefaultMoviePaths;

	UPROPERTY(Config, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MinimumLoadingScreenDisplayTime;
};