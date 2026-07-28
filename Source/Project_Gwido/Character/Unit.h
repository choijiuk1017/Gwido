// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Unit.generated.h"

class UAttributeComponent;
class UCombatComponent;
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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ReceiveHit(float Damage, AActor* DamageCauser);

	virtual void Die();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	bool IsHostileTo(const AUnit* OtherUnit) const;

	UAttributeComponent* GetAttributeComponent() const;
	UCombatComponent* GetCombatComponent() const;
	UPostureComponent* GetPostureComponent() const;
	UStateComponent* GetStateComponent() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPostureComponent> PostureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStateComponent> StateComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	EUnitFaction Faction = EUnitFaction::Neutral;
private:

};
