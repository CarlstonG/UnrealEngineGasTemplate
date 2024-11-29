// Copyright Whizz Entertianment


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/Widget/Controller/OverlayWidgetController.h"

//will contruct if its not initialize but if yes it will not 
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
				if (OverlayWidgetController == nullptr)
				{
					OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
					OverlayWidgetController->SetWidgetControllerParams(WCParams);


					OverlayWidgetController->BindCallbacksToDependencies();
					return OverlayWidgetController;
				}
				return OverlayWidgetController;
}
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
				checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
				checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

				UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
				//Widget->AddToViewport();
				OverlayWidget = Cast<UAuraUserWidget>(Widget);

				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
				UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
				
				//binding to HUD UI
				OverlayWidget->SetWidgetController(WidgetController); // On BP overlay
				WidgetController->BroadcastInitialValues(); // On BP health and mana globe
				Widget->AddToViewport();
}
