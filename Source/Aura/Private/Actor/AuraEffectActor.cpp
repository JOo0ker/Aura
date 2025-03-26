// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GamePlayEffectClass)
{
	if (const auto TargetAsc= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		check(GamePlayEffectClass);
		auto EffectContextHandle = TargetAsc->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const auto EffectSpecHandle = TargetAsc->MakeOutgoingSpec(GamePlayEffectClass, 1.0f, EffectContextHandle);
		TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	}
}
