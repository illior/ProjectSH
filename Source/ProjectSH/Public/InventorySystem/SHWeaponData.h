// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/SHItemData.h"
#include "SHCoreTypes.h"
#include "SHWeaponData.generated.h"

UCLASS()
class PROJECTSH_API USHWeaponData : public USHItemData
{
	GENERATED_BODY()


public:
	int32 GetCurrentAmmoInClip() const { return CurrentAmmoInClip; };
	int32 GetFastAccessIndex() const { return FastAccessIndex; };
	bool GetIsEquip() const { return bIsEquip; };

	UFUNCTION(BlueprintCallable, Category = "ItemData")
	int32 GetItemMaxAmmoInClip() const;
	UFUNCTION(BlueprintCallable, Category = "ItemData")
	ESHAmmoType GetItemAmmoType() const;

	virtual void Initialize(USHBaseItem* InItem, int32 InCount, FIntPoint InPosition) override;

	virtual bool IsFull() const override;
	virtual bool CanDrop() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem", meta = (ClampMin = "0"))
	int32 CurrentAmmoInClip = 1;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "WeaponData")
	int32 FastAccessIndex = -1;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "WeaponData")
	bool bIsEquip = false;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
