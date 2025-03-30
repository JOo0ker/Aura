// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGamePlayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	AURA_GAMEPLAY_TAG_REG(Primary, Strength, "Increases physical damage.")
	AURA_GAMEPLAY_TAG_REG(Primary, Intelligence, "Increases magical damage.")
	AURA_GAMEPLAY_TAG_REG(Primary, Resilience, "Increases Armor and Armor Penetration.")
	AURA_GAMEPLAY_TAG_REG(Primary, Vigor, "Increases Health.")

	/*
	 * Secondary Attributes
	 */
	AURA_GAMEPLAY_TAG_REG(Secondary, Armor, "Reduces damage taken, improves Block Chance.")
	AURA_GAMEPLAY_TAG_REG(Secondary, ArmorPenetration, "Ignored Percentage of enemy Armor, Increases Critical Hit Chance.")
	AURA_GAMEPLAY_TAG_REG(Secondary, BlockChance, "Chance to cut incoming damage in half.")
	AURA_GAMEPLAY_TAG_REG(Secondary, CriticalHitChance, "Chance to Double Damage plus critical hit bonus.")
	AURA_GAMEPLAY_TAG_REG(Secondary, CriticalHitDamage, "Bonus damage added when a critical hit is scored.")
	AURA_GAMEPLAY_TAG_REG(Secondary, CriticalHitResistance, "Reduces Critical Hit Chance of attacking enemies.")
	AURA_GAMEPLAY_TAG_REG(Secondary, HealthRegeneration, "Amount of Health regenerated every 1 second.")
	AURA_GAMEPLAY_TAG_REG(Secondary, ManaRegeneration, "Amount of Mana regenerated every 1 second.")
	AURA_GAMEPLAY_TAG_REG(Secondary, MaxHealth, "Maximum amount of Health obtainable.")
	AURA_GAMEPLAY_TAG_REG(Secondary, MaxMana, "Maximum amount of Mana obtainable.")
}
