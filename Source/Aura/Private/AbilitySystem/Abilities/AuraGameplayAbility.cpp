// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage</>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(const float InLevel) const
{
	float ManaCost = 0.0f;

	if (const auto CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(const float InLevel) const
{
	float Cooldown = 0.0f;

	if (const auto CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}

	return Cooldown;
}
