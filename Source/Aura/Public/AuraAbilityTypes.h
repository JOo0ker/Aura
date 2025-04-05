#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsBlockHit() const { return bIsBlockHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }

	FORCEINLINE void SetIsBlockHit(const bool bInIsCriticalHit) { bIsBlockHit = bInIsCriticalHit; }
	FORCEINLINE void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		const auto NewContext = new FAuraGameplayEffectContext;
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}

		return NewContext;
	}
	
protected:
	UPROPERTY()
	bool bIsBlockHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};