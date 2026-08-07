// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerCombatComponent.h"

#include "Character/PlayerCharacter.h"
#include "DataAsset/ComboData.h"
#include "DataAsset/WeaponCombatData.h"
#include "DataAsset/ItemData/WeaponItemData.h"
#include "Actor/Weapon/Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerCombatComponent::InitializeWeapons(const TArray<TObjectPtr<UWeaponItemData>>& InWeaponDatas)
{
	if (!OwnerPlayer) return;

	WeaponEquipDatas = InWeaponDatas;

	if (WeaponEquipDatas.IsEmpty()) return;

	SpawnedWeapons.Empty();

	for (UWeaponItemData* WeaponData : WeaponEquipDatas)
	{
		if (!WeaponData || !WeaponData->Weapon)
		{
			SpawnedWeapons.Add(nullptr);
			continue;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPlayer;

		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(
			WeaponData->Weapon,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (!SpawnedWeapon)
		{
			SpawnedWeapons.Add(nullptr);
			continue;
		}

		SpawnedWeapon->AttachToComponent(OwnerPlayer->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("Weapon"));

		SpawnedWeapon->SetActorHiddenInGame(true);

		SpawnedWeapon->SetActorEnableCollision(false);

		SpawnedWeapons.Add(SpawnedWeapon);
	}

	if (SpawnedWeapons.IsValidIndex(0) &&IsValid(SpawnedWeapons[0]))
	{
		CurrentWeaponIndex = 0;

		CurrentEquipWeaponData = WeaponEquipDatas[0];

		EquippedWeapon = SpawnedWeapons[0];

		EquippedWeapon->SetActorHiddenInGame(false);
	}
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

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	if (!PreviousWeaponData || !PreviousWeaponData->UnequipMontage)
	{
		if (NewWeaponData->EquipMontage)
		{
			AnimInstance->Montage_Play(NewWeaponData->EquipMontage);
		}

		return;
	}

	AnimInstance->Montage_Play(PreviousWeaponData->UnequipMontage);

	FOnMontageEnded EndDelegate;

	EndDelegate.BindLambda([this, NewWeaponData](UAnimMontage* Montage,bool bInterrupted)
	{
			if (!OwnerPlayer || bInterrupted) return;

			UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

			if (!AnimInstance) return;

			if (NewWeaponData && NewWeaponData->EquipMontage)
			{
				AnimInstance->Montage_Play(NewWeaponData->EquipMontage);
			}
	}
	);

	AnimInstance->Montage_SetEndDelegate(EndDelegate,PreviousWeaponData->UnequipMontage);

}

UComboData* UPlayerCombatComponent::GetCurrentComboData() const
{
	if (!CurrentWeaponData)
	{
		return nullptr;
	}

	return CurrentWeaponData->ComboData;
}

void UPlayerCombatComponent::ChangeWeapon(int32 NewWeaponIndex, UWeaponCombatData* NewWeaponCombatData)
{
	if (!OwnerPlayer) return;
	if (!NewWeaponCombatData) return;
	if (!WeaponEquipDatas.IsValidIndex(NewWeaponIndex)) return;
	if (!SpawnedWeapons.IsValidIndex(NewWeaponIndex)) return;

	UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	ResetCombo();

	PendingWeaponIndex = NewWeaponIndex;
	PendingWeaponData = NewWeaponCombatData;

	if (!CurrentWeaponData || !CurrentWeaponData->UnequipMontage)
	{
		CurrentWeaponIndex = PendingWeaponIndex;
		CurrentEquipWeaponData = WeaponEquipDatas[PendingWeaponIndex];
		CurrentWeaponData = PendingWeaponData;

		if (CurrentWeaponData->EquipMontage)
		{
			AnimInstance->Montage_Play(CurrentWeaponData->EquipMontage);
		}

		return;
	}

	UAnimMontage* UnequipMontage = CurrentWeaponData->UnequipMontage;

	AnimInstance->Montage_Play(UnequipMontage);

	FOnMontageEnded EndDelegate;

	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
			if (bInterrupted || !OwnerPlayer) return;

			if (!WeaponEquipDatas.IsValidIndex(PendingWeaponIndex)) return;

			UAnimInstance* AnimInstance = OwnerPlayer->GetMesh()->GetAnimInstance();

			if (!AnimInstance) return;

			CurrentWeaponIndex = PendingWeaponIndex;
			CurrentEquipWeaponData = WeaponEquipDatas[PendingWeaponIndex];

			CurrentWeaponData = PendingWeaponData;

			if (CurrentWeaponData && CurrentWeaponData->EquipMontage)
			{
				AnimInstance->Montage_Play(CurrentWeaponData->EquipMontage);
			}
	});

	AnimInstance->Montage_SetEndDelegate(EndDelegate,UnequipMontage);
}

void UPlayerCombatComponent::HideCurrentWeapon()
{
	if (!IsValid(EquippedWeapon)) return;

	EquippedWeapon->SetActorHiddenInGame(true);
}

void UPlayerCombatComponent::ShowPendingWeapon()
{
	if (!SpawnedWeapons.IsValidIndex(PendingWeaponIndex)) return;

	if (!WeaponEquipDatas.IsValidIndex(PendingWeaponIndex)) return;

	AWeapon* NewWeapon = SpawnedWeapons[PendingWeaponIndex];

	if (!IsValid(NewWeapon)) return;

	EquippedWeapon = NewWeapon;
	CurrentWeaponIndex = PendingWeaponIndex;
	CurrentEquipWeaponData = WeaponEquipDatas[PendingWeaponIndex];

	EquippedWeapon->SetActorHiddenInGame(false);

	PendingWeaponIndex = INDEX_NONE;
	PendingWeaponData = nullptr;
}