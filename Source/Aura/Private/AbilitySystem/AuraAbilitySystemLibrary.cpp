// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	if (const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		const auto AvatarActor = ASC->GetAvatarActor();
		
		const auto CharacterClassInfo = AuraGameMode->CharacterClassInfo;
		const auto [PrimaryAttributes] = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		auto PrimaryAttributesContextHandle = ASC->MakeEffectContext();
		PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(PrimaryAttributes, Level, PrimaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

		auto SecondaryAttributesContextHandle = ASC->MakeEffectContext();
		SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

		auto VitalAttributesContextHandle = ASC->MakeEffectContext();
		VitalAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
}
