
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PersistentState.generated.h"

class USaveGame; 

UINTERFACE(Blueprintable, BlueprintType)
class UPersistentState : public UInterface
{
	GENERATED_BODY()
};

class IPersistentState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGuid GetGuid() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USaveGame* GetData() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetData(const USaveGame* SaveGame);
};