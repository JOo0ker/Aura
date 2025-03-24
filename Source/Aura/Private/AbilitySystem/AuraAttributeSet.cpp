// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(70.f);
	InitMaxHealth(100.f);
	InitMana(60.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	AURA_ATTRIBUTE_NOTIFY(Health)
	AURA_ATTRIBUTE_NOTIFY(MaxHealth)
	AURA_ATTRIBUTE_NOTIFY(Mana)
	AURA_ATTRIBUTE_NOTIFY(MaxMana)
}

AURA_ATTRIBUTE_IMPL(Health)
AURA_ATTRIBUTE_IMPL(MaxHealth)
AURA_ATTRIBUTE_IMPL(Mana)
AURA_ATTRIBUTE_IMPL(MaxMana)
