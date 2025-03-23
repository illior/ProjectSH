// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SHCoreTypes.h"
#include "SHCraftingList.generated.h"

UCLASS(BlueprintType)
class PROJECTSH_API USHCraftingList : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:
	bool GetCraftRecipe(USHBaseItem* FirstItem, USHBaseItem* SecondItem, FSHCraftRecipe& CraftRecipe) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventorySystem")
	TArray<FSHCraftRecipe> Crafts;
};
