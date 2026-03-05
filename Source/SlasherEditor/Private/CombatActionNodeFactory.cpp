// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatActionNodeFactory.h"
#include "CombatActionNode.h"

UCombatActionNodeFactory::UCombatActionNodeFactory()
{
	SupportedClass = UCombatActionNode::StaticClass();

	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UCombatActionNodeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(InClass->IsChildOf(UCombatActionNode::StaticClass()));
	return NewObject<UCombatActionNode>(InParent, InClass, InName, Flags);
}
