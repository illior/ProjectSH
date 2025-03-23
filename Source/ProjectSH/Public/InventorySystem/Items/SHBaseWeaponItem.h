// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Items/SHBaseItem.h"
#include "SHCoreTypes.h"
#include "SHBaseWeaponItem.generated.h"

UCLASS()
class PROJECTSH_API USHBaseWeaponItem : public USHBaseItem
{
	GENERATED_BODY()


public:
	int32 GetMaxAmmoInClip() const { return MaxAmmoInClip; }
	ESHAmmoType GetAmmoType() const { return AmmoType; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "1"))
	int32 MaxAmmoInClip = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	ESHAmmoType AmmoType = ESHAmmoType::Pistol;
};
