// Copyright Whizz Entertainment

#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
    // Set up character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Disable controller-based rotations (handled by movement)
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
}

/*
// Ability system start replication
*/

void AAuraCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Init ability actor info for the Server
    InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Init ability actor info for the Client
    InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
    // Ensure PlayerState is valid
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    if (!AuraPlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("AuraPlayerState is NULL in InitAbilityActorInfo!"));
        return;
    }

    // Initialize Ability System Component
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is NULL in InitAbilityActorInfo!"));
        return;
    }

    // Initialize Attribute Set
    AttributeSet = AuraPlayerState->GetAttributeSet();
    if (!AttributeSet)
    {
        UE_LOG(LogTemp, Error, TEXT("AttributeSet is NULL in InitAbilityActorInfo!"));
        return;
    }

    // Initialize Ability Actor Info
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

    // UserWidget Init (HUD setup, only if the controller and HUD are valid)
    if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
        {
            AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AuraHUD is NULL in InitAbilityActorInfo!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AuraPlayerController is NULL in InitAbilityActorInfo!"));
    }
}
