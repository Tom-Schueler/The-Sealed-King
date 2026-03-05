
#include "Subsystems/MoviePlayerSubsystem/MoviePlayerProjectSettings.h"

UMoviePlayerProjectSettings::UMoviePlayerProjectSettings()
{
	DefaultLoadingScreenWidget = nullptr;
	DefaultMoviePaths.Empty();
}

#if WITH_EDITOR

void UMoviePlayerProjectSettings::SanitizeMoviePath(FString& InOutPath)
{
	if (InOutPath.IsEmpty())
	{ return; }
		
	static FString MoviesBase = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Movies"));
	FPaths::NormalizeDirectoryName(MoviesBase);
	MoviesBase += TEXT("/");

	FString Full = FPaths::ConvertRelativePathToFull(InOutPath);
	FPaths::NormalizeFilename(Full);

	// Only sanitize if the file is inside Content/Movies
	if (!Full.StartsWith(MoviesBase))
	{ return; }

	// Make relative to MoviesBase
	FString Relative = Full;
	FPaths::MakePathRelativeTo(Relative, *MoviesBase);

	// Remove extension (keep folders)
	Relative = FPaths::GetPath(Relative) / FPaths::GetBaseFilename(Relative);

	// Remove leading slashes just in case
	Relative.RemoveFromStart(TEXT("/"));
	Relative.RemoveFromStart(TEXT("\\"));

	InOutPath = Relative;
}

void UMoviePlayerProjectSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static const FName MoviePathsName = GET_MEMBER_NAME_CHECKED(UMoviePlayerProjectSettings, DefaultMoviePaths);

	if (PropertyChangedEvent.GetPropertyName() == MoviePathsName)
	{
		for (FFilePath& Path : DefaultMoviePaths)
		{
			SanitizeMoviePath(Path.FilePath);
		}
	}
}

void UMoviePlayerProjectSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	static const FName MoviePathsName = GET_MEMBER_NAME_CHECKED(UMoviePlayerProjectSettings, DefaultMoviePaths);

	// Inner struct member name: FFilePath::FilePath
	static const FName FilePathName = GET_MEMBER_NAME_CHECKED(FFilePath, FilePath);

	// If the changed leaf is "FilePath" and it belongs to "DefaultMoviePaths", sanitize immediately
	const FName ChangedLeafName = PropertyChangedEvent.GetPropertyName();

	FProperty* MemberProperty = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()
		? PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()
		: nullptr;

	if (ChangedLeafName == FilePathName && MemberProperty && MemberProperty->GetFName() == MoviePathsName)
	{
		// Try to only update the edited element
		const int32 Index = PropertyChangedEvent.GetArrayIndex(MoviePathsName.ToString());

		if (DefaultMoviePaths.IsValidIndex(Index))
		{
			SanitizeMoviePath(DefaultMoviePaths[Index].FilePath);
		}
		else
		{
			// Fallback: sanitize all
			for (FFilePath& Path : DefaultMoviePaths)
			{
				SanitizeMoviePath(Path.FilePath);
			}
		}
	}
}

#endif // WITH_EDITOR