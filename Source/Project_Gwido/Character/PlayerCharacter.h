// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Unit.h"
#include "PlayerCharacter.generated.h"

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
	void RequestBaseAttack();

#pragma endregion

protected:

	virtual void BeginPlay() override;

	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

};
