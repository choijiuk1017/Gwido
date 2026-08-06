// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponent.h"
#include "Enum/WeaponTypes.h"
#include "Delegates/DelegateCombinations.h"

#include "PlayerCombatComponent.generated.h"

class APlayerCharacter;
class UWeaponCombatData;
class UComboData;
class UAnimMontage;


UCLASS()
class PROJECT_GWIDO_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:



	UComboData* GetCurrentComboData() const;

#pragma region BaseAttack

	UPROPERTY(EditAnywhere, Category = "Combat|Combo")
	float ComboCheckTime = 0.45f;

	void SetWeaponCombatData(UWeaponCombatData* NewWeaponData);

	void BaseAttack();

#pragma endregion
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnerPlayer;

#pragma region BaseAttack

	UPROPERTY()
	TObjectPtr<UWeaponCombatData> CurrentWeaponData;

	int32 CurrentComboCount = 0;

	bool bHasComboInput = false;

	FTimerHandle ComboTimerHandle;

	void ComboStart();

	void ComboEnd(UAnimMontage* Montage, bool bInterrupted);

	void ComboCheck();

	void SetComboTimer();

	void ResetCombo();

	void HandleWeaponDataChanged(UWeaponCombatData* PreviousWeaponData, UWeaponCombatData* NewWeaponData);


#pragma endregion


};
