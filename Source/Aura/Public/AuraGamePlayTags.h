// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define STR(X) #X
#define STR2(X) STR(X)

#define STR_COM(X, Y) X##Y

#define AURA_GAMEPLAY_TAG_DECL_1(Name)\
FGameplayTag Name;

#define AURA_GAMEPLAY_TAG_REG_1(Name, Tip)\
	GameplayTags.Name = UGameplayTagsManager::Get().AddNativeGameplayTag(\
		FName(#Name),\
		FString(Tip)\
	);

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

// Attributes
#define AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Attributes, Category, Name)

#define AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Attributes, Category, Name, Tip)

// Combat Socket
#define AURA_GAMEPLAY_COMBAT_SOCKET_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_2(CombatSocket, Name)

#define AURA_GAMEPLAY_COMBAT_SOCKET_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_2(CombatSocket, Name, Tip)

// Input
#define AURA_GAMEPLAY_INPUT_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_2(InputTag, Name)

#define AURA_GAMEPLAY_INPUT_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_2(InputTag, Name, Tip)

// Damage
#define AURA_GAMEPLAY_DAMAGE_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_2(Damage, Name)\
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Resistance, Name##Resistance)

#define AURA_GAMEPLAY_DAMAGE_TAG_REG(Name, Tip, TipResistance)\
	AURA_GAMEPLAY_TAG_REG_2(Damage, Name, Tip);\
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Resistance, Name##Resistance, TipResistance);\
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage##Name, GameplayTags.Attributes##Resistance##Name##Resistance);

// Effect
#define AURA_GAMEPLAY_EFFECT_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_2(Effects, Name)

#define AURA_GAMEPLAY_EFFECT_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_2(Effects, Name, Tip)

// Abilities
#define AURA_GAMEPLAY_ABILITIES_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_2(Abilities, Name)

#define AURA_GAMEPLAY_ABILITIES_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_2(Abilities, Name, Tip)

// Abilities Type
#define AURA_GAMEPLAY_ABILITIES_TYPE_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Abilities, Type, Name)

#define AURA_GAMEPLAY_ABILITIES_TYPE_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Abilities, Type, Name, Tip)

// Abilities Status
#define AURA_GAMEPLAY_ABILITIES_STATUS_TAG_DECL(Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Abilities, Status, Name)

#define AURA_GAMEPLAY_ABILITIES_STATUS_TAG_REG(Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Abilities, Status, Name, Tip)

// Abilities Attack
#define AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Abilities, Category, Name)

#define AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Abilities, Category, Name, Tip)

// Cooldown
#define AURA_GAMEPLAY_COOLDOWN_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Cooldown, Category, Name)

#define AURA_GAMEPLAY_COOLDOWN_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Cooldown, Category, Name, Tip)


// Montage
#define AURA_GAMEPLAY_MONTAGE_TAG_DECL(Category, Name)\
	AURA_GAMEPLAY_TAG_DECL_3(Montage, Category, Name)

#define AURA_GAMEPLAY_MONTAGE_TAG_REG(Category, Name, Tip)\
	AURA_GAMEPLAY_TAG_REG_3(Montage, Category, Name, Tip)

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	FAuraGameplayTags();

	static FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Strength);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Intelligence);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Resilience);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Primary, Vigor);

	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, Armor);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, ArmorPenetration);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, BlockChance);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitChance);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitDamage);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, CriticalHitResistance);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, HealthRegeneration);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, ManaRegeneration);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, MaxHealth);
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Secondary, MaxMana);
	
	AURA_GAMEPLAY_ATTRIBUTE_TAG_DECL(Meta, IncomingXP);

	AURA_GAMEPLAY_INPUT_TAG_DECL(LMB);
	AURA_GAMEPLAY_INPUT_TAG_DECL(RMB);
	AURA_GAMEPLAY_INPUT_TAG_DECL(1);
	AURA_GAMEPLAY_INPUT_TAG_DECL(2);
	AURA_GAMEPLAY_INPUT_TAG_DECL(3);
	AURA_GAMEPLAY_INPUT_TAG_DECL(4);
	AURA_GAMEPLAY_INPUT_TAG_DECL(Passive_1);
	AURA_GAMEPLAY_INPUT_TAG_DECL(Passive_2);

	AURA_GAMEPLAY_TAG_DECL_1(Damage);
	AURA_GAMEPLAY_DAMAGE_TAG_DECL(Fire);
	AURA_GAMEPLAY_DAMAGE_TAG_DECL(Lightning);
	AURA_GAMEPLAY_DAMAGE_TAG_DECL(Arcane);
	AURA_GAMEPLAY_DAMAGE_TAG_DECL(Physical);

	AURA_GAMEPLAY_ABILITIES_TAG_DECL(None);
	
	AURA_GAMEPLAY_ABILITIES_TAG_DECL(Attack);
	AURA_GAMEPLAY_ABILITIES_TAG_DECL(Summon);
	AURA_GAMEPLAY_ABILITIES_TAG_DECL(HitReact);

	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_DECL(Locked);
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_DECL(Eligible);
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_DECL(UnLocked);
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_DECL(Equipped);

	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_DECL(Offensive);
	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_DECL(Passive);
	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_DECL(None);
	
	AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_DECL(Fire, FireBolt);
	AURA_GAMEPLAY_COOLDOWN_TAG_DECL(Fire, FireBolt);

	AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_DECL(Lightning, Electrocute);
	AURA_GAMEPLAY_COOLDOWN_TAG_DECL(Lightning, Electrocute);

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	AURA_GAMEPLAY_EFFECT_TAG_DECL(HitReact);

	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_DECL(Weapon);
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_DECL(RightHand);
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_DECL(LeftHand);
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_DECL(Tail);

	AURA_GAMEPLAY_MONTAGE_TAG_DECL(Attack, 1);
	AURA_GAMEPLAY_MONTAGE_TAG_DECL(Attack, 2);
	AURA_GAMEPLAY_MONTAGE_TAG_DECL(Attack, 3);
	AURA_GAMEPLAY_MONTAGE_TAG_DECL(Attack, 4);

private:
	static FAuraGameplayTags GameplayTags;
};
