#include "Actor/AuraInventorySystem.h"
#include "GameplayTagContainer.h"
#include "Engine/Engine.h"

UAuraInventorySystem::UAuraInventorySystem()
{
    // Constructor logic (if needed)
}

void UAuraInventorySystem::AddItem(TSubclassOf<AAuraEffectActor> EffectClass)
{
    if (!Inventory.Contains(EffectClass))
    {
        Inventory.Add(EffectClass);

        // Debug log to output the class name to the console
        UE_LOG(LogTemp, Log, TEXT("Added item: %s"), *EffectClass->GetName());

        // Add on-screen message for visual feedback
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Added item: %s"), *EffectClass->GetName()));
        }
    }
    else
    {
        // Debug log if item already exists
        UE_LOG(LogTemp, Warning, TEXT("Item already exists: %s"), *EffectClass->GetName());
    }
}

void UAuraInventorySystem::RemoveItem(TSubclassOf<AAuraEffectActor> EffectClass)
{
    if (Inventory.Contains(EffectClass))
    {
        Inventory.Remove(EffectClass);

        // Debug log to show the removed item
        UE_LOG(LogTemp, Log, TEXT("Removed item: %s"), *EffectClass->GetName());

        // Add on-screen message for visual feedback
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Removed item: %s"), *EffectClass->GetName()));
        }
    }
    else
    {
        // Debug log if item is not found
        UE_LOG(LogTemp, Warning, TEXT("Item not found: %s"), *EffectClass->GetName());
    }
}

bool UAuraInventorySystem::HasItem(TSubclassOf<AAuraEffectActor> EffectClass) const
{
    if (Inventory.Contains(EffectClass))
    {
        // Debug log if the item exists
        UE_LOG(LogTemp, Log, TEXT("Item found: %s"), *EffectClass->GetName());
        return true;
    }
    else
    {
        // Debug log if the item is not found
        UE_LOG(LogTemp, Warning, TEXT("Item not found: %s"), *EffectClass->GetName());
        return false;
    }
}

AAuraEffectActor* UAuraInventorySystem::GetEffectFromInventory(TSubclassOf<AAuraEffectActor> EffectClass) const
{
    // Create the effect actor instance if it's in the inventory
    if (Inventory.Contains(EffectClass))
    {
        // Log to confirm spawning the effect
        UE_LOG(LogTemp, Log, TEXT("Spawning effect: %s"), *EffectClass->GetName());

        // Add on-screen message for visual feedback
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Spawning effect: %s"), *EffectClass->GetName()));
        }

        return GetWorld()->SpawnActor<AAuraEffectActor>(EffectClass);
    }

    // Log if effect is not found in inventory
    UE_LOG(LogTemp, Warning, TEXT("Effect not found in inventory: %s"), *EffectClass->GetName());
    return nullptr;
}
