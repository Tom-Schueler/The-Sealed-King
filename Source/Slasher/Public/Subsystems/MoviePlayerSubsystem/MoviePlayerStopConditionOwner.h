
#pragma once

#include "UObject/Interface.h"
#include "MoviePlayerStopConditionOwner.generated.h"

class UMoviePlayerSubsystem;

DECLARE_DYNAMIC_DELEGATE_OneParam(FConditionMetDelegate, const UObject*, Self);

UINTERFACE(BlueprintType)
class UMoviePlayerStopConditionOwner : public UInterface
{
    GENERATED_BODY()
};

class IMoviePlayerStopConditionOwner
{
    GENERATED_BODY()

public:

    virtual void RegisterConditionMet(UMoviePlayerSubsystem* MoviePlayerSubsystem, const FName& InFunctionName) = 0;
};