// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_BestProtectPosition.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"  
#include "CoreMinimal.h"
#include "AIModule.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

UEnvQueryTest_BestProtectPosition::UEnvQueryTest_BestProtectPosition()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	TestPurpose = EEnvTestPurpose::Score;
	maxProtectionDistance.DefaultValue = 1000.f;
}

void UEnvQueryTest_BestProtectPosition::RunTest(FEnvQueryInstance& queryInstance) const
{
	UE_LOG(LogTemp, Display, TEXT("Running on screen eqs test!"));

	FVector closestActorLocation = FVector::Zero();

	TArray<FVector> targetLocations;
	
	queryInstance.PrepareContext(target, targetLocations);

	FindClosestActorToTarget(queryInstance, closestActorLocation, targetLocations);

	if (targetLocations.Num() <= 0) return;
	FVector targetPosition = targetLocations[0];

	UObject* owner = queryInstance.Owner.Get();

	FloatValueMin.BindData(owner, queryInstance.QueryID);
	float minThreasholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(owner, queryInstance.QueryID);
	float maxThreasholdValue = FloatValueMax.GetValue();

	maxProtectionDistance.BindData(owner, queryInstance.QueryID);
	float maxProtectionDistanceValue = maxProtectionDistance.GetValue();

	for (FEnvQueryInstance::ItemIterator iterator(this, queryInstance); iterator; ++iterator)
	{
		auto itemLocation = GetItemLocation(queryInstance, iterator);

		auto distanceBetween = (closestActorLocation - itemLocation).Length();

		if (closestActorLocation == FVector::Zero() || distanceBetween > maxProtectionDistanceValue)
		{
			distanceBetween = 0;
		}

		iterator.SetScore(TestPurpose, FilterType, - distanceBetween, minThreasholdValue, maxThreasholdValue);
	}
}

void UEnvQueryTest_BestProtectPosition::FindClosestActorToTarget(FEnvQueryInstance& queryInstance, FVector& closestActorLocation, TArray<FVector>& targetLocations) const
{
	double distanceToClosestActor = INFINITY;
	
	if (targetLocations.Num() <= 0) return;
	
	FVector targetLocation = targetLocations[0];

	for (TActorIterator<AActor> iterator(GetWorld(), actorBlueprintReference); iterator; ++iterator)
	{
		AActor* actor = *iterator;

		auto distance = (actor->GetActorLocation() - targetLocation).Length();
		
		if (distance < distanceToClosestActor)
		{
			distanceToClosestActor = distance;
			closestActorLocation = actor->GetActorLocation();
			UE_LOG(LogTemp, Display, TEXT("ClosestActor found!"));
		}
	}
}
