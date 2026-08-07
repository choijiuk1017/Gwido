// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemData", GetFName());
	}

	UPROPERTY(EditAnywhere, Category = "Base")
	TObjectPtr<UTexture2D> ItemTexture;

	UPROPERTY(EditAnywhere, Category = "Base")
	FString ItemName;
};
