// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_ChangeWeapon.h"

#include "Character/PlayerCharacter.h"
#include "Component/PlayerCombatComponent.h"

void UAnimNotify_ChangeWeapon::Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!Player) return;

	UPlayerCombatComponent* CombatComponent = Player->GetCombatComponent();

	if (!CombatComponent) return;

	CombatComponent->ApplyPendingWeaponChange();
}