// Copyright Whizz Entertianment

#pragma once

#include "CoreMinimal.h"
//header module abilityinterface
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"


//make sure to declare this if using pointers
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface //added IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:

	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//shared values and pointers
		// Ability system component, handles abilities like skills, spells, etc.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attribute set, manages the stats/attributes like health, mana, etc.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UAttributeSet> AttributeSet;
	//this is macro bind for ability tags and ability system
	virtual void InitAbilityActorInfo();
};
