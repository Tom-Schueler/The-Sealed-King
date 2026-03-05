// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatActionRootFactory.h"
#include "CombatActionRoot.h"

UCombatActionRootFactory::UCombatActionRootFactory()
{
	SupportedClass = UCombatActionRoot::StaticClass();

	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCombatActionRootFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UCombatActionRoot::StaticClass()));
	return NewObject<UCombatActionRoot>(InParent, InClass, InName, Flags);
}
