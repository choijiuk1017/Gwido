// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"

class APlayerCharacter;
class AWeapon;

class UComboData;
class UWeaponCombatData;
class UWeaponItemData;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_GWIDO_API UPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerCombatComponent();

	void BaseAttack();

	void InitializeWeapons(const TArray<TObjectPtr<UWeaponItemData>>& InWeaponDatas);

	void SetWeaponCombatData(UWeaponCombatData* NewWeaponData);

	void ChangeWeapon(int32 NewWeaponIndex, UWeaponCombatData* NewWeaponCombatData);

	void HideCurrentWeapon();

	void ShowPendingWeapon();


protected:

	virtual void BeginPlay() override;

private:
	void ComboStart();

	void ComboEnd(UAnimMontage* Montage,bool bInterrupted);

	void ComboCheck();

	void SetComboTimer();

	void ResetCombo();

	UComboData* GetCurrentComboData() const;

	void HandleWeaponDataChanged(UWeaponCombatData* PreviousWeaponData,UWeaponCombatData* NewWeaponData);


private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnerPlayer;

	UPROPERTY()
	TObjectPtr<UWeaponCombatData> CurrentWeaponData;

	UPROPERTY()
	TObjectPtr<UWeaponCombatData> PendingWeaponData;

	UPROPERTY()
	TArray<TObjectPtr<UWeaponItemData>> WeaponEquipDatas;

	UPROPERTY()
	TObjectPtr<UWeaponItemData> CurrentEquipWeaponData;

	UPROPERTY()
	TArray<TObjectPtr<AWeapon>> SpawnedWeapons;

	UPROPERTY()
	TObjectPtr<AWeapon> EquippedWeapon;

	int32 CurrentWeaponIndex = INDEX_NONE;

	int32 PendingWeaponIndex = INDEX_NONE;

	bool bIsChangingWeapon = false;

	int32 CurrentComboCount = 0;

	bool bHasComboInput = false;

	FTimerHandle ComboTimerHandle;
};
