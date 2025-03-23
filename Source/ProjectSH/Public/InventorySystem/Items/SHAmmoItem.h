// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Items/SHBaseItem.h"
#include "SHCoreTypes.h"
#include "SHAmmoItem.generated.h"

UCLASS()
class PROJECTSH_API USHAmmoItem : public USHBaseItem
{
	GENERATED_BODY()
	

public:
	ESHAmmoType GetAmmoType() const { return AmmoType; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo Item")
	ESHAmmoType AmmoType = ESHAmmoType::Pistol;
};
