#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actor/AuraEffectActor.h" // Assuming GameEffect is your base class for effects
#include "AuraInventorySystem.generated.h"

// Inventory System for storing and using items
UCLASS(Blueprintable)
class AURA_API UAuraInventorySystem : public UObject
{
	GENERATED_BODY()
public:
	UAuraInventorySystem();

	// Add Item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(TSubclassOf<AAuraEffectActor> EffectClass);

	// Remove Item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(TSubclassOf<AAuraEffectActor> EffectClass);

	// Check if Item exists
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(TSubclassOf<AAuraEffectActor> EffectClass) const;

	// Get the Effect from inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AAuraEffectActor* GetEffectFromInventory(TSubclassOf<AAuraEffectActor> EffectClass) const;

private:
	UPROPERTY()
	TArray<TSubclassOf<AAuraEffectActor>> Inventory;
};
