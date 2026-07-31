// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

#pragma region overrides

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma endregion

#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
#pragma endregion 

private:

	void InputMove(const FInputActionValue& Value);
	void InputLook(const FInputActionValue& Value);

	APlayerCharacter* GetPlayerCharacter() const;

};
