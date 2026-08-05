// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:

#pragma region BaseAttack

	int32 CurrentComboCount = 0;

	FTimerHandle ComboTimerHandle;

	bool bHasComboInput = false;

	UPROPERTY(EditAnywhere, Category = "Combat|Combo")
	float ComboCheckTime = 0.45f;

	int32 CurrentWeaponIndex = 0;

	void BaseAttack();

	void ComboStart();

	void ComboEnd(UAnimMontage* Montage, bool bInterrupted);

	void ComboCheck();

	void SetComboTimer();

#pragma endregion
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnerPlayer;
};
