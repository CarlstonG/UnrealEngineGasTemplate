// Copyright Whizz Entertainment

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface class to define enemy-related behaviors like highlighting
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:
	// Function to highlight the enemy actor
	virtual void HighlightActor() = 0;

	// Function to unhighlight the enemy actor
	virtual void UnHighlightActor() = 0;
};
