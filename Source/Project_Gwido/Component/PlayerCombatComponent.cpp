// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerCombatComponent.h"

#include "Character/PlayerCharacter.h"
#include "DataAsset/ComboData.h"
#include "DataAsset/WeaponCombatData.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerCombatComponent::SetWeaponCombatData(UWeaponCombatData* NewWeaponData)
{
	if (!IsValid(NewWeaponData)) return;

	if (CurrentWeaponData == NewWeaponData) return;

	ResetCombo();

	UWeaponCombatData* PreviousWeaponData = CurrentWeaponData;
	CurrentWeaponData = NewWeaponData;

	HandleWeaponDataChanged(PreviousWeaponData,CurrentWeaponData);
}

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
	//UCharacterMovementComponent* CharacterMovement = OwnerPlayer->GetCharacterMovement();
	//if (CharacterMovement)
	//{
	//	CharacterMovement->SetMovementMode(EMovementMode::MOVE_None);
	//}

	UComboData* ComboData = GetCurrentComboData();

	if (!ComboData || !ComboData->ComboMontage) return;

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(ComboData->ComboMontage);

	FOnMontageEnded  EndDelegate;
	EndDelegate.BindUObject(this, &UPlayerCombatComponent::ComboEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboData->ComboMontage);

	ComboTimerHandle.Invalidate();
	SetComboTimer();
}

void UPlayerCombatComponent::ComboEnd(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentComboCount = 0;
	bHasComboInput = false;
	OwnerPlayer->bIsAttacking = false;

	UCharacterMovementComponent* CharacterMovement = OwnerPlayer->GetCharacterMovement();
	/*if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	}*/
}

void UPlayerCombatComponent::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (!bHasComboInput) return;

	UComboData* ComboData = GetCurrentComboData();

	if (!ComboData || !ComboData->ComboMontage)
	{
		return;
	}

	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, ComboData->MaxComboCount);

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	const FName SectionName =
		*FString::Printf(TEXT("%s%d"), *ComboData->SectionPrefix, CurrentComboCount);

	AnimInstance->Montage_JumpToSection(SectionName, ComboData->ComboMontage);

	SetComboTimer();
	bHasComboInput = false;
}

void UPlayerCombatComponent::SetComboTimer()
{
	if (!OwnerPlayer) return;

	UComboData* ComboData = GetCurrentComboData();

	if (!ComboData || !ComboData->ComboMontage) return;

	const int32 ComboIndex = CurrentComboCount - 1;

	if (!ComboData->ComboFrame.IsValidIndex(ComboIndex)) return;

	const float AttackSpeedRate = 1.0f;
	const float ComboAvailableTime =
		(ComboData->ComboFrame[ComboIndex] / ComboData->FrameRate) / AttackSpeedRate;

	if (ComboAvailableTime > 0.0f)
	{
		OwnerPlayer->GetWorld()->GetTimerManager().SetTimer(
			ComboTimerHandle,
			this,
			&UPlayerCombatComponent::ComboCheck,
			ComboAvailableTime,
			false
		);
	}

}

void UPlayerCombatComponent::ResetCombo()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
	}

	ComboTimerHandle.Invalidate();

	CurrentComboCount = 0;
	bHasComboInput = false;

	if (OwnerPlayer)
	{
		OwnerPlayer->bIsAttacking = false;
	}
}



void UPlayerCombatComponent::HandleWeaponDataChanged(UWeaponCombatData* PreviousWeaponData, UWeaponCombatData* NewWeaponData)
{
	if (!OwnerPlayer || !CurrentWeaponData) return;

	// 무기 변경 시 로직
}

UComboData* UPlayerCombatComponent::GetCurrentComboData() const
{
	if (!CurrentWeaponData)
	{
		return nullptr;
	}

	return CurrentWeaponData->ComboData;
}