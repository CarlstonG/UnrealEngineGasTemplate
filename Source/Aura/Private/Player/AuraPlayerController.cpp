// Copyright Whizz Entertainment

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Perform cursor tracing to determine mouse position
    CursorTrace();

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // Check if the character is moving
        if (ControlledPawn->GetVelocity().SizeSquared() > 0)
        {
            // Movement is detected; do not lock to the mouse cursor
            return;
        }

        // Character is stationary; look at the mouse cursor
        FVector MouseWorldLocation, MouseWorldDirection;
        if (DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
        {
            // Trace to find where the mouse cursor intersects the ground plane
            const FVector TraceStart = MouseWorldLocation;
            const FVector TraceEnd = MouseWorldLocation + MouseWorldDirection * 10000.f; // Large distance
            FHitResult HitResult;

            if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
            {
                FVector LookAtLocation = HitResult.ImpactPoint;
                FVector CharacterLocation = ControlledPawn->GetActorLocation();
                FVector Direction = (LookAtLocation - CharacterLocation).GetSafeNormal();
                FRotator LookAtRotation = Direction.Rotation();

                FRotator CurrentRotation = ControlledPawn->GetActorRotation();
                FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, DeltaTime, 10.0f); // Smooth rotation speed
                ControlledPawn->SetActorRotation(SmoothRotation);
            }
        }
    }
}



void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(AuraContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(Subsystem);
    Subsystem->AddMappingContext(AuraContext, 0);

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}
//cursor trace
void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            // Case B: Highlight new actor
            ThisActor->HighlightActor();
        }
    }
    else
    {
        if (ThisActor == nullptr)
        {
            // Case C: Unhighlight previous actor
            LastActor->UnHighlightActor();
        }
        else
        {
            if (LastActor != ThisActor)
            {
                // Case D: Switch highlight to new actor
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
        }
    }
}


void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    if (InputAxisVector.IsNearlyZero())
    {
        return; // No movement input
    }

    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // Apply movement input
        const FVector MovementDirection = ForwardDirection * InputAxisVector.Y + RightDirection * InputAxisVector.X;
        ControlledPawn->AddMovementInput(MovementDirection, 1.0f);

        // Smoothly rotate character to face movement direction
        if (!MovementDirection.IsNearlyZero())
        {
            FRotator TargetRotation = MovementDirection.Rotation();
            FRotator CurrentRotation = ControlledPawn->GetActorRotation();
            FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // 10.0f is rotation speed
            ControlledPawn->SetActorRotation(SmoothRotation);
        }
    }
}
