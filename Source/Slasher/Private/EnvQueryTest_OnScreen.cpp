// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTest_OnScreen.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "AIModule.h"


UEnvQueryTest_OnScreen::UEnvQueryTest_OnScreen()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	TestPurpose = EEnvTestPurpose::Score;
	ScoringEquation = EEnvTestScoreEquation::Linear;
	FilterType = EEnvTestFilterType::Maximum;
}

void UEnvQueryTest_OnScreen::RunTest(FEnvQueryInstance& queryInstance) const
{
	UE_LOG(LogTemp, Display, TEXT("Running on screen eqs test!"));

	FVector2D viewportSize;
	queryInstance.World->GetGameViewport()->GetViewportSize(viewportSize);
	float viewportCenterX = viewportSize.X / 2;
	float viewportSizeY = viewportSize.Y / 2;

	UObject* owner = queryInstance.Owner.Get();

	FloatValueMin.BindData(owner, queryInstance.QueryID);
	float minThreasholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(owner, queryInstance.QueryID);
	float maxThreasholdValue = FloatValueMax.GetValue();

	FVector screenCenterWorldDirection;
	FVector screentCenterWorldLocation;

	queryInstance.World->GetFirstPlayerController()->DeprojectScreenPositionToWorld(viewportCenterX, viewportSizeY, screentCenterWorldLocation, screenCenterWorldDirection);

	for (FEnvQueryInstance::ItemIterator iterator(this, queryInstance); iterator; ++iterator) 
	{
		auto itemLoaction = GetItemLocation(queryInstance, iterator);

		// get direction from item location to screen center
		FVector directionToCenter = (itemLoaction - screentCenterWorldLocation).GetSafeNormal();

		auto dotProduct = directionToCenter.Dot(screenCenterWorldDirection);

		iterator.SetScore(TestPurpose, FilterType, dotProduct, minThreasholdValue, maxThreasholdValue);
	}
}
