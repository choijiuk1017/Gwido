// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerCombatComponent.h"

#include "Character/PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UPlayerCombatComponent::BaseAttack()
{
	if (!OwnerPlayer) return;

	if (OwnerPlayer->IsDead()) return;

	if (CurrentComboCount == 0)
	{
		ComboStart();

		OwnerPlayer->bIsAttacking = true;
		return;
	}

	if (ComboTimerHandle.IsValid())
	{
		bHasComboInput = true;
	}
	else
	{
		bHasComboInput = false;
	}

}

void UPlayerCombatComponent::ComboStart()
{
	if (!OwnerPlayer) return;

	CurrentComboCount = 1;
	UCharacterMovementComponent* CharacterMovement = OwnerPlayer->GetCharacterMovement();
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_None);
	}

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(OwnerPlayer->ComboMontage);

	FOnMontageEnded  EndDelegate;
	EndDelegate.BindUObject(this, &UPlayerCombatComponent::ComboEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, OwnerPlayer->ComboMontage);

	ComboTimerHandle.Invalidate();
	SetComboTimer();
}

void UPlayerCombatComponent::ComboEnd(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentComboCount = 0;
	bHasComboInput = false;
	OwnerPlayer->bIsAttacking = false;

	UCharacterMovementComponent* CharacterMovement = OwnerPlayer->GetCharacterMovement();
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UPlayerCombatComponent::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (!bHasComboInput) return;

	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, OwnerPlayer->MaxComboCount);

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	const FName SectionName(
		*FString::Printf(TEXT("%s%d"), "Attack", CurrentComboCount));

	AnimInstance->Montage_JumpToSection(SectionName, OwnerPlayer->ComboMontage);

	SetComboTimer();
	bHasComboInput = false;
}

void UPlayerCombatComponent::SetComboTimer()
{
	if (!OwnerPlayer)
	{
		return;
	}

	if (ComboCheckTime <= 0.0f)
	{
		return;
	}

	OwnerPlayer->GetWorldTimerManager().SetTimer(
		ComboTimerHandle,
		this,
		&UPlayerCombatComponent::ComboCheck,
		ComboCheckTime,
		false
	);

}

