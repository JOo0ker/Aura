// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			const auto PS = PC->GetPlayerState<AAuraPlayerState>();
			const auto Asc = PS->GetAbilitySystemComponent();
			const auto AS = PS->GetAttributeSet();
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = Asc;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	if (FWidgetControllerParams WCParams;
		MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	if (FWidgetControllerParams WCParams;
		MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	
	return nullptr;
}


USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	if (FWidgetControllerParams WCParams;
		MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	
	return nullptr;
}


void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
	if (const auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const auto AvatarActor = ASC->GetAvatarActor();

		// ReSharper disable once CppUseStructuredBinding
		const auto DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		auto PrimaryAttributesContextHandle = ASC->MakeEffectContext();
		PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(DefaultInfo.PrimaryAttributes, Level,
		                                                               PrimaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

		auto SecondaryAttributesContextHandle = ASC->MakeEffectContext();
		SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
			CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

		auto VitalAttributesContextHandle = ASC->MakeEffectContext();
		VitalAttributesContextHandle.AddSourceObject(AvatarActor);
		const auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level,
		                                                             VitalAttributesContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
                                                     ECharacterClass CharacterClass)
{
	if (const auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (const auto AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}

		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			for (auto AbilityClass : CharacterClassInfo->GetClassDefaultInfo(CharacterClass).StartupAbilities)
			{
				auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
		return AuraGameMode->CharacterClassInfo;

	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}

	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& EffectContextHandle,
                                              const bool bInIsBlockHit)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockHit(bInIsBlockHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                 const bool bInIsCriticalHit)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetAlivePlayerWithRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlappingActors,
                                                         const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                         const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);

		for (const auto& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;

	return !bFriends;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
	const auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	if (!CharacterClassInfo) return 0;

	const auto& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const auto XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
