// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MoviePlayerSubsystem/MoviePlayerSubsystem.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerProjectSettings.h"
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerStopConditionOwner.h"
#include "Blueprint/UserWidget.h"

void UMoviePlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DefaultLoadingScreenWidget = nullptr;
	DefaultMoviePaths.Empty();
	MinimumLoadingScreenDisplayTime = 0;

	const UMoviePlayerProjectSettings* settings = GetDefault<UMoviePlayerProjectSettings>();
	if (settings)
	{
		DefaultLoadingScreenWidget = settings->DefaultLoadingScreenWidget;
		MinimumLoadingScreenDisplayTime = settings->MinimumLoadingScreenDisplayTime;

		TArray<FString> Paths;
		for (const FFilePath& MoviePath : settings->DefaultMoviePaths)
		{
			const FString& Path = MoviePath.FilePath;
			Paths.Add(Path);
		}
		DefaultMoviePaths = Paths;
	}	
}

void UMoviePlayerSubsystem::Deinitialize()
{
	FSlateApplication::Get().OnPreTick().Remove(Handle);
	Super::Deinitialize();
}

bool UMoviePlayerSubsystem::StartLoadingScreen(const TSubclassOf<UUserWidget> loadingScreenWidget, const TArray<FString>& moviePaths)
{
	IGameMoviePlayer* MoviePlayer = nullptr;
	if (!GetMoviePlayerHelper(MoviePlayer))
	{ return false; }

	if (loadingScreenWidget)
	{
		if (UWorld* World = GetWorld())
		{
			LoadingWidgetInstance = CreateWidget<UUserWidget>(World, loadingScreenWidget);
			if (LoadingWidgetInstance)
			{
				LoadingSlateWidget = LoadingWidgetInstance->TakeWidget();
			}
		}
	}

	FLoadingScreenAttributes attributes;
	attributes.bAllowEngineTick = false;
	attributes.bAllowInEarlyStartup = false;
	attributes.bMoviesAreSkippable = false;
	attributes.bWaitForManualStop = true;
	attributes.MoviePaths = moviePaths;
	attributes.PlaybackType = EMoviePlaybackType::MT_Looped;
	attributes.WidgetLoadingScreen = LoadingSlateWidget.IsValid() ? LoadingSlateWidget.ToSharedRef() : FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	MoviePlayer->SetupLoadingScreen(attributes);
	bool StartedPlaying = MoviePlayer->PlayMovie();
	if(!StartedPlaying)
	{ return false; }

	LoadingScreenStartTime = FPlatformTime::Seconds();
	LoadFinished = false;
	Handle = FSlateApplication::Get().OnPreTick().AddUObject(this, &UMoviePlayerSubsystem::LoadingScreenTick);
	return true;
}

void UMoviePlayerSubsystem::LoadingScreenTick(float DeltaTime)
{
	if (!LoadFinished)
	{ return; }

	const double Elapsed = FPlatformTime::Seconds() - LoadingScreenStartTime;

	if (Elapsed >= (double)MinimumLoadingScreenDisplayTime)
	{
		EndLoadingScreen();
		FSlateApplication::Get().OnPreTick().Remove(Handle);
	}
}

bool UMoviePlayerSubsystem::StartDefaultLoadingScreen()
{
	return StartLoadingScreen(DefaultLoadingScreenWidget, DefaultMoviePaths);
}

bool UMoviePlayerSubsystem::AddFinishCondition(const TScriptInterface<UMoviePlayerStopConditionOwner>& ConditionOwner)
{
	IGameMoviePlayer* MoviePlayer = nullptr;
	if (!GetMoviePlayerHelper(MoviePlayer))
	{ return false; }

	return AddFinishConditionRaw(ConditionOwner);
}

bool UMoviePlayerSubsystem::AddFinishConditions(const TArray<TScriptInterface<UMoviePlayerStopConditionOwner>>& ConditionOwners)
{
	IGameMoviePlayer* MoviePlayer = nullptr;
	if (!GetMoviePlayerHelper(MoviePlayer))
	{ return false; }

	for (const TScriptInterface<UMoviePlayerStopConditionOwner>& ConditionOwner : ConditionOwners)
	{
		if (!AddFinishConditionRaw(ConditionOwner))
		{ return false; }
	}

	return true;
}

void UMoviePlayerSubsystem::OnFinishConditionMet(const UObject* ConditionOwner)
{
	if (!ConditionOwner)
	{ return; }

	CachedConditionOwners.Remove(ConditionOwner);

	if (CachedConditionOwners.IsEmpty())
	{ 
		LoadFinished = true;
	}
}

bool UMoviePlayerSubsystem::AddFinishConditionRaw(const TScriptInterface<UMoviePlayerStopConditionOwner>& InConditionOwner)
{
	if(!InConditionOwner)
	{ return false; }

	UObject* Obj = InConditionOwner.GetObject();

	bool* IsAlreadyInSet = nullptr;
	CachedConditionOwners.Add(Obj, IsAlreadyInSet);
	if (IsAlreadyInSet)
	{ return false; }

	IMoviePlayerStopConditionOwner* ConditionOwner = Cast<IMoviePlayerStopConditionOwner>(Obj);
	ConditionOwner->RegisterConditionMet(this, FName(TEXT("OnFinishConditionMet")));
	return true;
}

bool UMoviePlayerSubsystem::GetMoviePlayerHelper(IGameMoviePlayer*& OutMoviePlayer)
{
	IGameMoviePlayer* moviePlayer = GetMoviePlayer();
	if (!moviePlayer)
	{ return false; }

	if (moviePlayer->IsMovieCurrentlyPlaying())
	{ return false; }

	OutMoviePlayer = moviePlayer;
	return true;
}

void UMoviePlayerSubsystem::EndLoadingScreen()
{
	IGameMoviePlayer* MoviePlayer = GetMoviePlayer();
	if (!MoviePlayer)
	{ return; }

	MoviePlayer->StopMovie();
	LoadingSlateWidget.Reset();
	LoadingWidgetInstance = nullptr;
	CachedConditionOwners.Empty();
}