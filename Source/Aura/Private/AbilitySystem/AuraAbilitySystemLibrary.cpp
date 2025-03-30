// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (const auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			const auto PS = PC->GetPlayerState<AAuraPlayerState>();
			const auto Asc = PS->GetAbilitySystemComponent();
			const auto AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, Asc, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (const auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			const auto PS = PC->GetPlayerState<AAuraPlayerState>();
			const auto Asc = PS->GetAbilitySystemComponent();
			const auto AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, Asc, AS);

			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}
