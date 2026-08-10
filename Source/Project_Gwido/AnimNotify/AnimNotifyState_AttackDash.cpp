// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_AttackDash.h"

#include "Character/PlayerCharacter.h"
#include "Component/PlayerCombatComponent.h"

void UAnimNotifyState_AttackDash::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (!Player) return;

	UPlayerCombatComponent* CombatComponent = Player->GetCombatComponent();

	if (!CombatComponent) return;

	CombatComponent->DashToNearestEnemy(SearchRadius, DashSpeed * FrameDeltaTime, StopDistance);
}