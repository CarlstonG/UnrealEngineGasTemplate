// Copyright Whizz Entertianment

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"



class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface; //  IEnemyInterface


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController();
	//create playertick to highlight enemy
	virtual void PlayerTick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	

private:

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;


	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);


	//highglight components
	void CursorTrace();
	TScriptInterface<IEnemyInterface> LastActor; // IEnemyInterface
	TScriptInterface<IEnemyInterface> ThisActor;
	

};
