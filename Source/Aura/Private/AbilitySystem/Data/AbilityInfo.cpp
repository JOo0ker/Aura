// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	const auto Res = AbilitiesInformation.FindByPredicate(
		[AbilityTag](const FAuraAbilityInfo& AbilityInfo)
		{
			if (AbilityInfo.AbilityTag == AbilityTag)
				return true;
			return false;
		});

	if (Res)
		return *Res;

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetName());
	}
	
	return FAuraAbilityInfo();
}
