// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define STR(X) #X
#define STR2(X) STR(X)

#define STR_COM(X, Y) X##Y

#define AURA_GAMEPLAY_TAG_DECL(Category, Name)\
	FGameplayTag Attributes##Category##Name;

#define AURA_GAMEPLAY_TAG_REG(Category, Name, Tip)\
	GameplayTags.Attributes##Category##Name = UGameplayTagsManager::Get().AddNativeGameplayTag(\
		FName(STR2(STR_COM(Attributes, .##Category##.##Name))),\
		FString(Tip)\
	);\

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	AURA_GAMEPLAY_TAG_DECL(Primary, Strength)
	AURA_GAMEPLAY_TAG_DECL(Primary, Intelligence)
	AURA_GAMEPLAY_TAG_DECL(Primary, Resilience)
	AURA_GAMEPLAY_TAG_DECL(Primary, Vigor)
	
	AURA_GAMEPLAY_TAG_DECL(Secondary, Armor)
	AURA_GAMEPLAY_TAG_DECL(Secondary, ArmorPenetration)
	AURA_GAMEPLAY_TAG_DECL(Secondary, BlockChance)
	AURA_GAMEPLAY_TAG_DECL(Secondary, CriticalHitChance)
	AURA_GAMEPLAY_TAG_DECL(Secondary, CriticalHitDamage)
	AURA_GAMEPLAY_TAG_DECL(Secondary, CriticalHitResistance)
	AURA_GAMEPLAY_TAG_DECL(Secondary, HealthRegeneration)
	AURA_GAMEPLAY_TAG_DECL(Secondary, ManaRegeneration)
	AURA_GAMEPLAY_TAG_DECL(Secondary, MaxHealth)
	AURA_GAMEPLAY_TAG_DECL(Secondary, MaxMana)

private:
	static FAuraGameplayTags GameplayTags;
};
