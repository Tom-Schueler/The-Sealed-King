// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatAction.h"
#include "CombatActionDirection.h"
#include "CombatActionNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SLASHER_API UCombatActionNode : public UObject
{
	GENERATED_BODY()

public:
	UCombatActionNode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool IsActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float KnockbackForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float PostureDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName WeaponHitboxPresetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TMap<ECombatActionDirection, TObjectPtr<UAnimMontage>> Montages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TMap<ECombatAction, TObjectPtr<UCombatActionNode>> Children;
};
