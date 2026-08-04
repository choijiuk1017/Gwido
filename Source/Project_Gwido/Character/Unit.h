// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Unit.generated.h"

class UAttributeComponent;
class UPostureComponent;
class UStateComponent;

UENUM(BlueprintType)
enum class EUnitFaction : uint8
{
	Player,
	Enemy,
	Neutral
};

UCLASS()
class PROJECT_GWIDO_API AUnit : public ACharacter
{
	GENERATED_BODY()


public:
	AUnit();

#pragma region Combat

	bool bIsAttacking = false;

	virtual void Die();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	bool IsHostileTo(const AUnit* OtherUnit) const;

	virtual void ReceiveHit(float Damage, AActor* DamageCauser);

#pragma endregion


protected:
	virtual void BeginPlay() override;
	
#pragma region Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPostureComponent> PostureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStateComponent> StateComponent;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	EUnitFaction Faction = EUnitFaction::Neutral;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
	bool bIsDead;


	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
};
