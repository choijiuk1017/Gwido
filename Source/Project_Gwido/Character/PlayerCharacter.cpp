// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "Component/PlayerCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
    CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    SpringArmComp->TargetArmLength = 200.0f;
    SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

    SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->bInheritPitch = true;
    SpringArmComp->bInheritYaw = true;
    SpringArmComp->bInheritRoll = false;

    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 15.0f;

    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.0f;

    CameraComp->bUsePawnControlRotation = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    Faction = EUnitFaction::Player;
}

#pragma region Movement
void APlayerCharacter::Move(const FVector2D& MovementInput)
{
    if (!Controller)
    {
        return;
    }

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementInput.Y);
    AddMovementInput(RightDirection, MovementInput.X);
}

void APlayerCharacter::Look(const FVector2D& LookInput)
{
    if (!Controller)
    {
        return;
    }

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::BeginSprint()
{
    if (bIsSprinting) return;

    bIsSprinting = true;

    GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::EndSprint()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

#pragma endregion

void APlayerCharacter::RequestBaseAttack()
{
    CombatComponent->BaseAttack();
}