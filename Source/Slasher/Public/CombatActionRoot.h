// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatAction.h"
#include "CombatActionNode.h"
#include "CombatActionRoot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SLASHER_API UCombatActionRoot : public UObject
{
	GENERATED_BODY()

public:
	UCombatActionRoot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TMap<ECombatAction, TObjectPtr<UCombatActionNode>> Children;
	
};
