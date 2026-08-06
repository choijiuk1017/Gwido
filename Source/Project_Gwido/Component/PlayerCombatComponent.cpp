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

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("공격 실행 / WeaponData: %s / ComboData: %s / ComboCount: %d"),
		*GetNameSafe(CurrentWeaponData),
		*GetNameSafe(GetCurrentComboData()),
		CurrentComboCount
	);

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

	if (!ComboData || !ComboData->ComboMontage) return;

	if (CurrentComboCount >= ComboData->MaxComboCount)
	{
		bHasComboInput = false;
		return;
	}

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	const FName CurrentSectionName(*FString::Printf(TEXT("%s%d"), *ComboData->SectionPrefix, CurrentComboCount));

	const int32 NextComboCount = CurrentComboCount + 1;

	const FName NextSectionName(*FString::Printf(TEXT("%s%d"), *ComboData->SectionPrefix, NextComboCount));

	AnimInstance->Montage_SetNextSection(CurrentSectionName, NextSectionName, ComboData->ComboMontage);

	CurrentComboCount = NextComboCount;
	bHasComboInput = false;

	SetComboTimer();
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