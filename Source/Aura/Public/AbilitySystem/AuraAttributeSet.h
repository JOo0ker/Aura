// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define AURA_ATTRIBUTE_DECL(X)\
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_##X, Category = "Vital Attributes")\
	FGameplayAttributeData X;\
	UFUNCTION()\
	void OnRep_##X(const FGameplayAttributeData& Old##X) const;\
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, X)

#define AURA_ATTRIBUTE_NOTIFY(X)\
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, X, COND_None, REPNOTIFY_Always);

#define AURA_ATTRIBUTE_IMPL(X)\
	void UAuraAttributeSet::OnRep_##X(const FGameplayAttributeData& Old##X) const\
	{\
		GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ##X, Old##X);\
	}

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	explicit FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	/* Start Source */
	UPROPERTY()
	UAbilitySystemComponent* SourceAsc = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	/* End Source */

	/* Start Target */
	UPROPERTY()
	UAbilitySystemComponent* TargetAsc = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	/* End Target */

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data);
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)
};
