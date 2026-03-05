// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_BestProtectPosition.generated.h"

/**
 * 
 */
UCLASS()
class SLASHER_API UEnvQueryTest_BestProtectPosition : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_BestProtectPosition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActorToProtect")
	TSubclassOf<AActor> actorBlueprintReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActorToAttack")
	TSubclassOf<UEnvQueryContext> target;

	UPROPERTY(EditDefaultsOnly, Category = "ProtectionDistance")
	FAIDataProviderFloatValue maxProtectionDistance;

protected:
	virtual void RunTest(FEnvQueryInstance& queryInstance) const override;
	
private:
	void FindClosestActorToTarget(FEnvQueryInstance& queryInstance, FVector& closestActorLocation, TArray<FVector>& contextLocations) const;
};
