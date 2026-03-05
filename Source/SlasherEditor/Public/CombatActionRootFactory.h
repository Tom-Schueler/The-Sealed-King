// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatActionRootFactory.generated.h"

/**
 * 
 */
UCLASS(HideCategories = Object)
class SLASHEREDITOR_API UCombatActionRootFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UCombatActionRootFactory();

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	//~ End UFactory Interface
};
