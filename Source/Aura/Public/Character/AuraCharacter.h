// Copyright Whizz Entertianment

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:

	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
private:
	//created this function so we will not replate long line of codes
	//commented this out as we also declared it on base
	/*void InitAbilityActorInfo();*/
	
	//this one can be added if base was also declared with corresponding InitAbility
	virtual void InitAbilityActorInfo() override;
};
