// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGamePlayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"

#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

void FEffectProperties::SetEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	EffectContextHandle = Data.EffectSpec.GetContext();
	SourceAsc = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(SourceAsc) && SourceAsc->AbilityActorInfo.IsValid()
		&& SourceAsc->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceController = SourceAsc->AbilityActorInfo->PlayerController.Get();
		SourceAvatarActor = SourceAsc->AbilityActorInfo->AvatarActor.Get();
		if (SourceController && SourceAvatarActor)
		{
			if (const auto Pawn = Cast<APawn>(SourceAvatarActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		if (SourceController)
		{
			SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);
	}
}

UAuraAttributeSet::UAuraAttributeSet()
{
	// ReSharper disable once CppUseStructuredBinding
	const auto& GameplayTags = FAuraGameplayTags::Get();

	AURA_ATTRIBUTE_MAP_ADD(Primary, Strength);
	AURA_ATTRIBUTE_MAP_ADD(Primary, Intelligence);
	AURA_ATTRIBUTE_MAP_ADD(Primary, Resilience);
	AURA_ATTRIBUTE_MAP_ADD(Primary, Vigor);

	AURA_ATTRIBUTE_MAP_ADD(Secondary, Armor)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, ArmorPenetration)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, BlockChance)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, CriticalHitChance)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, CriticalHitDamage)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, CriticalHitResistance)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, HealthRegeneration)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, ManaRegeneration)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, MaxHealth)
	AURA_ATTRIBUTE_MAP_ADD(Secondary, MaxMana)

	AURA_ATTRIBUTE_MAP_ADD(Resistance, FireResistance)
	AURA_ATTRIBUTE_MAP_ADD(Resistance, LightningResistance)
	AURA_ATTRIBUTE_MAP_ADD(Resistance, ArcaneResistance)
	AURA_ATTRIBUTE_MAP_ADD(Resistance, PhysicalResistance)
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	AURA_ATTRIBUTE_NOTIFY(Strength)
	AURA_ATTRIBUTE_NOTIFY(Intelligence)
	AURA_ATTRIBUTE_NOTIFY(Resilience)
	AURA_ATTRIBUTE_NOTIFY(Vigor)

	AURA_ATTRIBUTE_NOTIFY(Armor)
	AURA_ATTRIBUTE_NOTIFY(ArmorPenetration)
	AURA_ATTRIBUTE_NOTIFY(BlockChance)
	AURA_ATTRIBUTE_NOTIFY(CriticalHitChance)
	AURA_ATTRIBUTE_NOTIFY(CriticalHitDamage)
	AURA_ATTRIBUTE_NOTIFY(CriticalHitResistance)
	AURA_ATTRIBUTE_NOTIFY(HealthRegeneration)
	AURA_ATTRIBUTE_NOTIFY(ManaRegeneration)
	AURA_ATTRIBUTE_NOTIFY(MaxHealth);
	AURA_ATTRIBUTE_NOTIFY(MaxMana);

	AURA_ATTRIBUTE_NOTIFY(Health);
	AURA_ATTRIBUTE_NOTIFY(Mana);

	AURA_ATTRIBUTE_NOTIFY(FireResistance)
	AURA_ATTRIBUTE_NOTIFY(LightningResistance)
	AURA_ATTRIBUTE_NOTIFY(ArcaneResistance)
	AURA_ATTRIBUTE_NOTIFY(PhysicalResistance)
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		const float LocalIncomingDamage = GetInComingDamage();
		SetInComingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (const auto CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
					CombatInterface->Die();

				SendXPEvent(Props);
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().EffectsHitReact);
				Props.TargetAsc->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bBlock = UAuraAbilitySystemLibrary::IsBlockHit(Props.EffectContextHandle);
			const bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);

			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCritical);
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		const float LocalIncomingXP = GetInComingXP();
		SetInComingXP(0.f);

		// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to ImcomingXP.
		if (Props.SourceCharacter->Implements<UCombatInterface>() && Props.SourceCharacter->Implements<UPlayerInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
			const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);

			if (const int32 NumLevelUps = NewLevel - CurrentLevel; NumLevelUps > 0)
			{
				const int32 AttributePointReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel);
				const int32 SpellPointReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel);
				
				IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointReward);
				IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointReward);

				bTopOfHealth = true;
				bTopOfMana = true;
				
				IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
			}
			
			IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
		}
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOfHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOfHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOfMana)
	{
		SetMana(GetMaxMana());
		bTopOfMana = false;
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit,
                                         bool bCriticalHit)
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (const auto PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}

		if (const auto PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	{
		// Source = causer of the effect, Target = target of the effect (owner of this AS)

		Props.EffectContextHandle = Data.EffectSpec.GetContext();
		Props.SourceAsc = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

		if (IsValid(Props.SourceAsc) && Props.SourceAsc->AbilityActorInfo.IsValid() && Props.SourceAsc->AbilityActorInfo
			->AvatarActor.IsValid())
		{
			Props.SourceAvatarActor = Props.SourceAsc->AbilityActorInfo->AvatarActor.Get();
			Props.SourceController = Props.SourceAsc->AbilityActorInfo->PlayerController.Get();
			if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
			{
				if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
				{
					Props.SourceController = Pawn->GetController();
				}
			}
			if (Props.SourceController)
			{
				Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
			}
		}

		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			
			Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
			Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
			Props.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.AttributesMetaIncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.AttributesMetaIncomingXP, Payload);
	}
}

AURA_ATTRIBUTE_IMPL(Strength)
AURA_ATTRIBUTE_IMPL(Intelligence)
AURA_ATTRIBUTE_IMPL(Resilience)
AURA_ATTRIBUTE_IMPL(Vigor)

AURA_ATTRIBUTE_IMPL(Armor)
AURA_ATTRIBUTE_IMPL(ArmorPenetration)
AURA_ATTRIBUTE_IMPL(BlockChance)
AURA_ATTRIBUTE_IMPL(CriticalHitChance)
AURA_ATTRIBUTE_IMPL(CriticalHitDamage)
AURA_ATTRIBUTE_IMPL(CriticalHitResistance)
AURA_ATTRIBUTE_IMPL(HealthRegeneration)
AURA_ATTRIBUTE_IMPL(ManaRegeneration)
AURA_ATTRIBUTE_IMPL(MaxHealth)
AURA_ATTRIBUTE_IMPL(MaxMana)

AURA_ATTRIBUTE_IMPL(Health)
AURA_ATTRIBUTE_IMPL(Mana)

AURA_ATTRIBUTE_IMPL(FireResistance)
AURA_ATTRIBUTE_IMPL(LightningResistance)
AURA_ATTRIBUTE_IMPL(ArcaneResistance)
AURA_ATTRIBUTE_IMPL(PhysicalResistance)
