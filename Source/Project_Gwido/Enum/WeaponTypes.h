// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponMode : uint8
{
    Sword   UMETA(DisplayName = "Sword"),
    Gun     UMETA(DisplayName = "Gun"),
    Spear   UMETA(DisplayName = "Spear")
};
