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
	InitHealth(70.f);
	InitMaxHealth(100.f);
	InitMana(73.f);
	InitMaxMana(99.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	AURA_ATTRIBUTE_NOTIFY(Health);
	AURA_ATTRIBUTE_NOTIFY(MaxHealth);
	AURA_ATTRIBUTE_NOTIFY(Mana);
	AURA_ATTRIBUTE_NOTIFY(MaxMana);
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
}

AURA_ATTRIBUTE_IMPL(Health)
AURA_ATTRIBUTE_IMPL(MaxHealth)
AURA_ATTRIBUTE_IMPL(Mana)
AURA_ATTRIBUTE_IMPL(MaxMana)
