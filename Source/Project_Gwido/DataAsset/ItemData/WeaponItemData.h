// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/ItemData/ItemData.h"
#include "Enum/WeaponTypes.h"
#include "WeaponItemData.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS()
class PROJECT_GWIDO_API UWeaponItemData : public UItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponMode WeaponMode = EWeaponMode::Sword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMesh> UnequipedWeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon> Weapon;
};
