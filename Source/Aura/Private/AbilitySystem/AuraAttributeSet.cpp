// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

#include "Net/UnrealNetwork.h"

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
	Props.SetEffectProperties(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
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

