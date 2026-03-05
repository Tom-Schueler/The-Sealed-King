// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CombatActionNodeFactory.generated.h"

/**
 * 
 */
UCLASS(HideCategories = Object)
class SLASHEREDITOR_API UCombatActionNodeFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UCombatActionNodeFactory();

	//~ Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	//~ End UFactory Interface
};
