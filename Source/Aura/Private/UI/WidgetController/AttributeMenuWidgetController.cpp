// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

	for (const auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				auto Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(GetAuraAS());
				AttributeInfoDelegate.Broadcast(Info);
			});
	}
	
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{

	check(AttributeInfo);

	for (const auto& Pair :  GetAuraAS()->TagsToAttributes)
	{
		auto Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue( GetAuraAS());
		AttributeInfoDelegate.Broadcast(Info);
	}
	
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttributes(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
