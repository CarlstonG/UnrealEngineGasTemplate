// Copyright Whizz Entertianment

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public: 
	AAuraEnemy();
   /*Enemy Interface*/

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	

	//for debug only
	//UPROPERTY(BlueprintReadOnly)
	//bool bHighlighted = false;
protected:
	virtual void BeginPlay() override;
};
