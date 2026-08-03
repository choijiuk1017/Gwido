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

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainPlayerController::InputMove);
	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainPlayerController::InputLook);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AMainPlayerController::InputSprint_Begin);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AMainPlayerController::InputSprint_End);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedCharacter = Cast<APlayerCharacter>(InPawn);
}

void AMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	CachedCharacter = nullptr;
}

void AMainPlayerController::InputMove(const FInputActionValue& Value)
{
	if (!CachedCharacter) return;

	const FVector2D MovementInput = Value.Get<FVector2D>();

	CachedCharacter->Move(MovementInput);
}

void AMainPlayerController::InputLook(const FInputActionValue& Value)
{
	if (!CachedCharacter) return;

	const FVector2D LookInput = Value.Get<FVector2D>();

	CachedCharacter->Look(LookInput);
}

void AMainPlayerController::InputSprint_Begin()
{
	if (!CachedCharacter) return;

	CachedCharacter->BeginSprint();
}

void AMainPlayerController::InputSprint_End()
{
	if (!CachedCharacter) return;

	CachedCharacter->EndSprint();
}

