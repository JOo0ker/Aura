// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGamePlayTags.h"

FString UAuraFireBolt::GetDescription(const int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().DamageFire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launched a bolt of fire, exploding on impact and dealing: </>"
			// Damage
			"<Damage>%d</>"
			"<Default> fire damage with a chance to brun</>"
			),
			
			// Values
			Level,
			ManaCost,
			Cooldown,
			Damage
			)
			;
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launched %d bolts of fire, exploding on impact and dealing: </>"
			
			// Damage
			"<Damage>%d</>"
			"<Default> fire damage with a chance to brun</>"
			),
			
			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage
			);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(const int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().DamageFire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
				// Title
				"<Title>NEXT LEVEL</>\n\n"

				// Level
				"<Small>Level: </><Level>%d</>\n"
				// ManaCost
				"<Small>ManaCost: </><ManaCost>%.1f</>\n"
				// Cooldown
				"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

				// Number of FireBolts
				"<Default>Launched %d bolts of fire, exploding on impact and dealing: </>"
			
				// Damage
				"<Damage>%d</>"
				"<Default> fire damage with a chance to brun</>"
				),
			
				// Values
				Level,
				ManaCost,
				Cooldown,
				FMath::Min(Level, NumProjectiles),
				Damage
				);
}