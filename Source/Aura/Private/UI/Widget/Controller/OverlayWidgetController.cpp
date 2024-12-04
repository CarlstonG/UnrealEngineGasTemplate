// Copyright Whizz Entertianment

#include "UI/Widget/Controller/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    // Broadcast initial values if the attribute set is valid
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}       

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    if (!AbilitySystemComponent || !AttributeSet)
    {
        UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent or AttributeSet is null!"));
        return;
    }

    // For broadcasting value change on attributes via Overlay
    const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        }
    );

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnManaChanged.Broadcast(Data.NewValue);
        }
    );
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxManaChanged.Broadcast(Data.NewValue);
        }
    );


    //Replaced by lambas
    //AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    //    AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

    //AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    //    AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

  /*  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);*/

  /*  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);*/

    // Broadcasting effect asset tags (Message popup logic is unchanged)
    Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags)
        {
            for (const FGameplayTag& Tag : AssetTags)
            {
                // For example, Tag = Message.HealthPotion
                // "Message.HealthPotion".MatchesTag("Message") will return true,
                // "Message".MatchesTag("Message.HealthPotion") will return false
                FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                if (Tag.MatchesTag(MessageTag))
                {
                    const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    MessageWidgetRowDelegate.Broadcast(*Row);
                }
            }
        }
    );
}
//Replaced by Lambas
// 
//void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
//{
//    OnHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
//{
//    OnMaxHealthChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data)
//{
//    OnManaChanged.Broadcast(Data.NewValue);
//}
//
//void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data)
//{
//    OnMaxManaChanged.Broadcast(Data.NewValue);
//}





void UOverlayWidgetController::ShieldChanged(const FOnAttributeChangeData& Data)
{
    OnShieldChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxShieldChanged(const FOnAttributeChangeData& Data)
{
    OnMaxShieldChanged.Broadcast(Data.NewValue);
}

// Add To Inventory Function with Debugging for Inventory
void UOverlayWidgetController::AddToInventory(FGameplayTag ItemTag, int32 Count)
{
    // Log the initial function call with ItemTag and Count for debugging
    UE_LOG(LogTemp, Log, TEXT("AddToInventory called with ItemTag: %s, Count: %d"), *ItemTag.ToString(), Count);

    // Search for an existing inventory slot with the same tag
    bool bFound = false;
    for (FInventorySlot& Slot : Inventory)
    {
        if (Slot.ItemTag == ItemTag)
        {
            // If found, increment the count
            Slot.Count += Count;
            bFound = true;
            break;
        }
    }

    // If not found, add a new inventory slot
    if (!bFound)
    {
        FInventorySlot NewSlot(ItemTag, Count);
        Inventory.Add(NewSlot);
    }

    // Optionally, update quick action bar after adding to inventory
    UpdateQuickActionBar();

    // Print for debugging (optional)
    UE_LOG(LogTemp, Log, TEXT("Added %d of item %s to inventory."), Count, *ItemTag.ToString());

    // Optional: Add On-Screen Debug Message to see in the game view
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Added %d of item %s"), Count, *ItemTag.ToString()));
}

void UOverlayWidgetController::UpdateQuickActionBar()
{
    // Clear the quick action bar first (optional, depending on your design)
    QuickActionBar.Empty();

    // Here you can define how to choose items from the inventory to populate the quick action bar
    // For example, let's just add the first 5 items from the inventory
    int32 MaxSlots = 5;  // You can change this number based on your quick action bar size
    for (int32 i = 0; i < FMath::Min(MaxSlots, Inventory.Num()); ++i)
    {
        QuickActionBar.Add(Inventory[i]);
    }

    // Broadcast an update to the UI (e.g., through a delegate)
    OnQuickActionBarUpdated.Broadcast(QuickActionBar);

    // Optionally, print the contents of the quick action bar for debugging
    for (const FInventorySlot& Slot : QuickActionBar)
    {
        UE_LOG(LogTemp, Log, TEXT("Quick Action Bar Slot: %s - Count: %d"), *Slot.ItemTag.ToString(), Slot.Count);
    }
}

