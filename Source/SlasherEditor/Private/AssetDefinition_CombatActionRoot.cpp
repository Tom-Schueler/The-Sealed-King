// Fill out your copyright notice in the Description page of Project Settings.

	
#include "AssetDefinition_CombatActionRoot.h"
#include "CombatActionRoot.h"

FText UAssetDefinition_CombatActionRoot::GetAssetDisplayName() const
{
	return FText::FromString(TEXT("Combat Action Root"));
}

FText UAssetDefinition_CombatActionRoot::GetAssetDescription(const FAssetData& AssetData) const
{
	return FText::FromString(TEXT("Holds information about the first Nodes."));
}

FLinearColor UAssetDefinition_CombatActionRoot::GetAssetColor() const
{
	return FLinearColor(FColor::FromHex("#8f1a5c"));
}

TSoftClassPtr<UObject> UAssetDefinition_CombatActionRoot::GetAssetClass() const
{
	return UCombatActionRoot::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_CombatActionRoot::GetAssetCategories() const
{
	static const auto Categories = {
		FAssetCategoryPath(FText::FromName("Combat")),
	}; // Make unique for both

	return Categories;
}
