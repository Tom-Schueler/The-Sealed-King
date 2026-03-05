
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CollectiblesSubsystem.generated.h"

class UCollectiblesSaveGame;

UCLASS()
class SLASHER_API UCollectiblesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Begin Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End Subsystem

	UFUNCTION(BlueprintCallable)
	bool SetCollected(const FName& ColectableName);

	UFUNCTION(BlueprintCallable)
	const TMap<FName, bool> GetCollectibles() const
	{
		return Collectibles;
	}

private:

	void HandleLoadPlayerCollection();
	void HandlePlayerCollectionLoaded(UCollectiblesSaveGame* PlayerCollection);

private:

	UPROPERTY(Transient)
	TMap<FName, bool> Collectibles;

	FDelegateHandle LoadPlayerCollectionHandle;

};