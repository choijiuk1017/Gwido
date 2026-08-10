// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackDash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API UAnimNotifyState_AttackDash : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SearchRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 100.0f;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
