// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGamePlayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

FAuraGameplayTags::FAuraGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	AURA_GAMEPLAY_TAG_REG_3(Attributes, Primary, Strength, "Increases physical damage.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Primary, Intelligence, "Increases magical damage.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Primary, Resilience, "Increases Armor and Armor Penetration.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Primary, Vigor, "Increases Health.")

	/*
	 * Secondary Attributes
	 */
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, Armor, "Reduces damage taken, improves Block Chance.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, ArmorPenetration,
	                                "Ignored Percentage of enemy Armor, Increases Critical Hit Chance.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, BlockChance, "Chance to cut incoming damage in half.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, CriticalHitChance, "Chance to Double Damage plus critical hit bonus.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, CriticalHitDamage, "Bonus damage added when a critical hit is scored.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, CriticalHitResistance,
	                                "Reduces Critical Hit Chance of attacking enemies.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, HealthRegeneration, "Amount of Health regenerated every 1 second.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, ManaRegeneration, "Amount of Mana regenerated every 1 second.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, MaxHealth, "Maximum amount of Health obtainable.")
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Secondary, MaxMana, "Maximum amount of Mana obtainable.")

	/*
 	* Meta Attributes
 	*/
	AURA_GAMEPLAY_ATTRIBUTE_TAG_REG(Meta, IncomingXP, "Incoming XP Meta Attribute.")

	/*
 	* Input 
 	*/
	AURA_GAMEPLAY_INPUT_TAG_REG(LMB, "Input Tag for Left Mouse Button.")
	AURA_GAMEPLAY_INPUT_TAG_REG(RMB, "Input Tag for Right Mouse Button.")
	AURA_GAMEPLAY_INPUT_TAG_REG(1, "Input Tag for 1 Key.")
	AURA_GAMEPLAY_INPUT_TAG_REG(2, "Input Tag for 2 Key.")
	AURA_GAMEPLAY_INPUT_TAG_REG(3, "Input Tag for 3 Key.")
	AURA_GAMEPLAY_INPUT_TAG_REG(4, "Input Tag for 4 Key.")
	AURA_GAMEPLAY_INPUT_TAG_REG(Passive_1, "Input Tag for Passive Ability 1.")
	AURA_GAMEPLAY_INPUT_TAG_REG(Passive_2, "Input Tag for Passive Ability 2.")

	/*
	 * Damage
	 */
	AURA_GAMEPLAY_TAG_REG_1(Damage, "Damage.")
	AURA_GAMEPLAY_DAMAGE_TAG_REG(Fire, "Fire Damage Type.", "Resistance to Fire damage")
	AURA_GAMEPLAY_DAMAGE_TAG_REG(Lightning, "Lightning Damage Type.", "Resistance to Lightning damage")
	AURA_GAMEPLAY_DAMAGE_TAG_REG(Arcane, "Arcane Damage Type.", "Resistance to Arcane damage")
	AURA_GAMEPLAY_DAMAGE_TAG_REG(Physical, "Physical Damage Type.", "Resistance to Physical damage")

	/*
	 * Effects
	 */
	AURA_GAMEPLAY_EFFECT_TAG_REG(HitReact, "Tag granted when Hit Reacting.")

	/*
	 * Abilities
	 */
	AURA_GAMEPLAY_ABILITIES_TAG_REG(None, "No Ability - like the nullptr for Ability Tags")
	AURA_GAMEPLAY_ABILITIES_TAG_REG(Attack, "Attack Ability Tag")
	AURA_GAMEPLAY_ABILITIES_TAG_REG(Summon, "Summon Ability Tag")
	AURA_GAMEPLAY_ABILITIES_TAG_REG(HitReact, "Hit React Ability1")
	
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_REG(Locked, "Locked Status")
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_REG(Eligible, "Eligible Status")
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_REG(UnLocked, "UnLocked Status")
	AURA_GAMEPLAY_ABILITIES_STATUS_TAG_REG(Equipped, "Equipped Status")

	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_REG(Offensive, "Type Offensive")
	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_REG(Passive, "Type Passive")
	AURA_GAMEPLAY_ABILITIES_TYPE_TAG_REG(None, "Type None")
	
	AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_REG(Fire, FireBolt, "FireBolt Ability Tag")
	AURA_GAMEPLAY_ABILITIES_ATTACK_TAG_REG(Lightning, Electrocute, "Electrocute Ability Tag")


	/*
	 * Cooldown
	 */
	AURA_GAMEPLAY_COOLDOWN_TAG_REG(Fire, FireBolt, "FireBolt Cooldown Tag")
	AURA_GAMEPLAY_COOLDOWN_TAG_REG(Lightning, Electrocute, "Electrocute Cooldown Tag")

	/*
	 * Combat Socket
	 */
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_REG(Weapon, "Weapon")
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_REG(RightHand, "Right Hand")
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_REG(LeftHand, "Left Hand")
	AURA_GAMEPLAY_COMBAT_SOCKET_TAG_REG(Tail, "Tail")

	/*
 	* Attack
 	*/
	AURA_GAMEPLAY_MONTAGE_TAG_REG(Attack, 1, "Attack 1")
	AURA_GAMEPLAY_MONTAGE_TAG_REG(Attack, 2, "Attack 2")
	AURA_GAMEPLAY_MONTAGE_TAG_REG(Attack, 3, "Attack 3")
	AURA_GAMEPLAY_MONTAGE_TAG_REG(Attack, 4, "Attack 4")
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
}
