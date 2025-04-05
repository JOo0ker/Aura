// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGamePlayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	/* Target */
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	/* Source */
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	static AuraDamageStatics& Get()
	{
		static AuraDamageStatics DamageStatics;
		return DamageStatics;
	}
	
private:
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);

		const auto Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.AttributesSecondaryArmor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.AttributesSecondaryBlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesSecondaryCriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesResistanceFireResistance, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesResistanceLightningResistance, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesResistanceArcaneResistance, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesResistancePhysicalResistance, PhysicalResistanceDef);

		TagsToCaptureDefs.Add(Tags.AttributesSecondaryArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Tags.AttributesSecondaryCriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.AttributesSecondaryCriticalHitDamage, CriticalHitDamageDef);
	}
};

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().ArmorDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().BlockChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().FireResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().LightningResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().PhysicalResistanceDef);

	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics::Get().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const auto SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const auto TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const auto& Spec = ExecutionParams.GetOwningSpec();
	auto EffectContextHandle = Spec.GetContext();

	const auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	const auto CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const auto DamageTypeTag = Pair.Key;
		const auto ResistanceTag = Pair.Value;

		checkf(AuraDamageStatics::Get().TagsToCaptureDefs.Contains(ResistanceTag),
		       TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage."), *ResistanceTag.ToString());
		const auto CaptureDef = AuraDamageStatics::Get().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		
		float Resistance =  0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		Damage += DamageTypeValue;
	}

	{
		// Capture BlockChance on Target, and determine if there was a successful Block
		float TargetBlockChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().BlockChanceDef,
		                                                           EvaluationParams,
		                                                           TargetBlockChance);
		TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.0f);

		// If Block, halve the damage.
		const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
		if (bBlocked) { Damage *= 0.5f; }
		UAuraAbilitySystemLibrary::SetIsBlockHit(EffectContextHandle, bBlocked);
	}

	{
		// ArmorPenetration ignores a percentage of the Target's Armor
		float TargetArmor = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().ArmorDef, EvaluationParams,
		                                                           TargetArmor);
		const auto ArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
			FName("EffectiveArmor"), FString());
		const auto ArmorCoefficient = ArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());


		float SourceArmorPenetration = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().ArmorPenetrationDef,
		                                                           EvaluationParams, SourceArmorPenetration);
		SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.0f);
		const auto ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
			FName("ArmorPenetration"), FString());
		const auto ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());


		const float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) /
			100.f;
		Damage *= (100.f - EffectiveArmor * ArmorCoefficient) / 100.f;
	}

	{
		// Critical
		float SourceCriticalHitChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().CriticalHitChanceDef,
		                                                           EvaluationParams, SourceCriticalHitChance);
		SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.0f);

		float SourceCriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().CriticalHitDamageDef,
		                                                           EvaluationParams, SourceCriticalHitDamage);
		SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 1.0f);

		float TargetCriticalResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics::Get().CriticalHitResistanceDef,
		                                                           EvaluationParams, TargetCriticalResistance);
		TargetCriticalResistance = FMath::Max<float>(TargetCriticalResistance, 0.0f);

		const auto CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
			FName("CriticalHitResistance"), FString());
		const auto CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(
			SourceCombatInterface->GetPlayerLevel());

		const bool bIsCriticalHit = FMath::RandRange(1, 100) < SourceCriticalHitChance - TargetCriticalResistance *
			CriticalHitResistanceCoefficient;

		if (bIsCriticalHit)
		{
			Damage = 2.f * Damage + SourceCriticalHitDamage;
		}

		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);
	}


	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
