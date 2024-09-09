// Copyright Whizz Entertianment


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
//#include <Character/AuraEnemy.h>


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}
//h defination if mouse hits something (highlight)
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

}
//h defination if mouse hits something (highlight)
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//if no actor hit do nothing
		/**
		Line trace from cursor. There are sevaral scenario:
		A. LastActor is null and ThisActor is null
		-do nothing
		B. LastActor is null and this actor is valid
		 -HighLight Actor
		C. LastActor is valid and Thisactor is null
		  -unhighlight lastactor
		D. both actors are valid but lastactor != thisactor
		 -unhighlight lastactor and highlight this actor
		E. Both actors are valid and are the same actor
		do nothing

	**/



	if (!CursorHit.bBlockingHit) return;
	//if statement above is false proceed below

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	//Cast<IEnemyInterface>(CursorHit.GetActor());



	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//case b
			ThisActor->HighlightActor();

		}
		else {
			// case a

		}
	}
	else // last actor is valid
	{
		//case c
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				//case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else //case e
			{
				//do thing
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

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



