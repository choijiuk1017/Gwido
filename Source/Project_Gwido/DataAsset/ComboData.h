// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API UComboData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Name)
	FString SectionPrefix;

	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = SkillPlayeRate)
	float ComboPlayRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> ComboFrame;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboMontage;
};
