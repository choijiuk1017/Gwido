// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Unit.h"

#include "Component/AttributeComponent.h"
#include "Component/CombatComponent.h"
#include "Component/PostureComponent.h"
#include "Component/StateComponent.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	PostureComponent = CreateDefaultSubobject<UPostureComponent>(TEXT("PostureComponent"));
	StateComponent = CreateDefaultSubobject<UStateComponent>(TEXT("StateComponent"));

}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnit::ReceiveHit(float Damage, AActor* DamageCauser)
{

}

void AUnit::Die()
{

}

bool AUnit::IsDead() const
{
	return false;
}

bool AUnit::IsHostileTo(const AUnit* OtherUnit) const
{
	return OtherUnit && Faction != OtherUnit->Faction;
}

UAttributeComponent* AUnit::GetAttributeComponent() const
{
	return AttributeComponent;
}

UCombatComponent* AUnit::GetCombatComponent() const
{
	return CombatComponent;
}

UStateComponent* AUnit::GetStateComponent() const
{
	return StateComponent;
}

UPostureComponent* AUnit::GetPostureComponent() const
{
	return PostureComponent;
}