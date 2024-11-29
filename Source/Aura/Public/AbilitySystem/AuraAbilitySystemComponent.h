// Copyright Whizz Entertainment

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
/**
 * Custom Ability System Component to handle gameplay effect applications.
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Call this when the Ability Actor Info is set up
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

protected:
	// Callback when a gameplay effect is applied
	UFUNCTION()
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};