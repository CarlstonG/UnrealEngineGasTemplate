#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Controller/AuraWidgetController.h"
#include "GameplayEffectTypes.h"
#include "OverlayWidgetController.generated.h"
// Inventory Slot Structure
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	// Default constructor
	FInventorySlot() : ItemTag(), Count(0) {
	}

	// Parameterized constructor
	FInventorySlot(FGameplayTag InItemTag, int32 InCount) : ItemTag(InItemTag), Count(InCount) {
	}
};

// UI Widget Row Structure
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UAuraUserWidget;
// Attribute Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldChangedSignature, float, NewShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxShieldChangedSignature, float, NewMaxShield);
// Message Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
// Inventory Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickActionBarUpdated, const TArray<FInventorySlot>&, QuickActionBar);
// Main Overlay Widget Controller
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// Virtual Functions
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// GAS Attributes Delegates
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;


	/*Shield*/

	UPROPERTY(BlueprintAssignable, Category= "GAS|Attributes")
	FOnShieldChangedSignature OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxShieldChangedSignature OnMaxShieldChanged;

	// GAS Message Delegate
	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	// Inventory System
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnQuickActionBarUpdated OnQuickActionBarUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToInventory(FGameplayTag ItemTag, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateQuickActionBar();



protected:
	// GAS Message Widget Data Table
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	// Inventory Data
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "Quick Action Bar")
	TArray<FInventorySlot> QuickActionBar;


	/* GAS Attribute Change Functions replaced with lamdas
	* 
	* 
	void HealthChanged(const FOnAttributeChangeData& Data);
	void MaxHealthChanged(const FOnAttributeChangeData& Data);
	void ManaChanged(const FOnAttributeChangeData& Data);
	void MaxManaChanged(const FOnAttributeChangeData& Data);
	*/
	void ShieldChanged(const FOnAttributeChangeData& Data);
	void MaxShieldChanged(const FOnAttributeChangeData& Data);


	// Generic Function for Retrieving Data Table Rows
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

// Implementation of Template Function
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}