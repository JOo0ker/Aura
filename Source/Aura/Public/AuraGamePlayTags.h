// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define STR(X) #X
#define STR2(X) STR(X)

#define STR_COM(X, Y) X##Y

#define AURA_GAMEPLAY_TAG_DECL_2(Category, Name)\
	FGameplayTag Category##Name;

#define AURA_GAMEPLAY_TAG_REG_2(Category, Name, Tip)\
	GameplayTags.Category##Name = UGameplayTagsManager::Get().AddNativeGameplayTag(\
		FName(STR(Category##.##Name)),\
		FString(Tip)\
	);

#define AURA_GAMEPLAY_TAG_DECL_3(Main, Category, Name)\
	FGameplayTag Main##Category##Name;

#define AURA_GAMEPLAY_TAG_REG_3(Main, Category, Name, Tip)\
	GameplayTags.Main##Category##Name = UGameplayTagsManager::Get().AddNativeGameplayTag(\
		FName(STR2(STR_COM(Main, .##Category##.##Name))),\
		FString(Tip)\
	);


#define AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Attributes, Category, Name)

#define AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Attributes, Category, Name, Tip)

#define AURA_GAMEPLAY_INPUT_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_2(Category, Name)

#define AURA_GAMEPLAY_INPUT_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_2(Category, Name, Tip)

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

	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Strength)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Intelligence)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Resilience)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Vigor)
	
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, Armor)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, ArmorPenetration)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, BlockChance)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitChance)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitDamage)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitResistance)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, HealthRegeneration)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, ManaRegeneration)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, MaxHealth)
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, MaxMana)

	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, LMB);
	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, RMB);
	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, 1);
	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, 2);
	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, 3);
	AURA_GAMEPLAY_INPUT_TAG_DECL(InputTag, 4);

private:
	static FAuraGameplayTags GameplayTags;
};
