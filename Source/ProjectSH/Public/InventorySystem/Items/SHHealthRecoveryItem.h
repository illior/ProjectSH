// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Items/SHBaseItem.h"
#include "InventorySystem/Interfaces/SHUsableItem.h"
#include "SHHealthRecoveryItem.generated.h"

UCLASS()
class PROJECTSH_API USHHealthRecoveryItem : public USHBaseItem, public ISHUsableItem
{
	GENERATED_BODY()
	

public:
	virtual bool CanUse(APawn* InPawn)const override;
	virtual void Use(APawn* InPawn) override;

	float GetHealAmount() { return HealAmount; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Recovery", meta = (ClampMin = "1"))
	float HealAmount = 100.0f;
};
