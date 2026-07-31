// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Character/PlayerCharacter.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::InputMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::InputLook);
}

void AMainPlayerController::InputMove(const FInputActionValue& Value)
{
	const FVector2D MovementInput = Value.Get<FVector2D>();

	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacter())
	{
		PlayerCharacter->Move(MovementInput);
	}
}

void AMainPlayerController::InputLook(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	AddYawInput(LookInput.X);
	AddPitchInput(LookInput.Y);
}

APlayerCharacter* AMainPlayerController::GetPlayerCharacter() const
{
	return Cast<APlayerCharacter>(GetPawn());
}