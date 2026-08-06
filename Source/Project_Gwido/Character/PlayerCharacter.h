// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Unit.h"
#include "PlayerCharacter.generated.h"

class UAnimMontage;
class UPlayerCombatComponent;
class UWeaponCombatData;

/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API APlayerCharacter : public AUnit
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

#pragma region Movement

	void Move(const FVector2D& MovementInput);

	void Look(const FVector2D& LookInput);
	
	void BeginSprint();

	void EndSprint();

#pragma endregion

#pragma region Combat

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<TObjectPtr<UWeaponCombatData>> WeaponCombatDatas;

	void RequestBaseAttack();

	void RequestChangeNextWeapon();

#pragma endregion

protected:

	virtual void BeginPlay() override;

#pragma region Movement

	bool bIsSprinting = false;

#pragma endregion

#pragma region Combat

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPlayerCombatComponent> CombatComponent;

#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;


};
