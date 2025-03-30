// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	const auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (const auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				auto Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (const auto& Pair : AS->TagsToAttributes)
	{
		auto Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
