
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldLoaderSubsystem.generated.h"

UCLASS()
class SLASHER_API UWorldLoaderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Begin Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End Subsystem
};