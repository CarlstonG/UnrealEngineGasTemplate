#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // Verify if the AbilitySystemComponent is valid before binding the delegate
    if (this)
    {
        OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

        // Debug message to indicate the delegate was successfully bound
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Delegate bound successfully in AbilityActorInfoSet!"));
    }
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    // Debug message to show the effect was applied
    
    /*GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green, TEXT("Effect Applied!"));*/
    
    //display tags
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    //for (const FGameplayTag& Tag : TagContainer)
    //{
    //    //TODO: Broadcast the tag to the Widget Controller
    //    const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
    //    GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, Msg);
    //}
    EffectAssetTags.Broadcast(TagContainer);
}
