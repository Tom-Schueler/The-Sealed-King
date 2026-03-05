// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetDefinition_CombatActionNode.h"
#include "CombatActionNode.h"

FText UAssetDefinition_CombatActionNode::GetAssetDisplayName() const
{
    return FText::FromString(TEXT("Combat Action Node"));
}

FText UAssetDefinition_CombatActionNode::GetAssetDescription(const FAssetData& AssetData) const
{
    return FText::FromString(TEXT("Node that holds information about the action and folowing actions"));
}

FLinearColor UAssetDefinition_CombatActionNode::GetAssetColor() const
{
    return FLinearColor(FColor::FromHex("#4d1111"));
}

TSoftClassPtr<UObject> UAssetDefinition_CombatActionNode::GetAssetClass() const
{
    return UCombatActionNode::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_CombatActionNode::GetAssetCategories() const
{
    static const auto Categories = {
        FAssetCategoryPath(FText::FromName("Combat")),
    };

    return Categories;
}
