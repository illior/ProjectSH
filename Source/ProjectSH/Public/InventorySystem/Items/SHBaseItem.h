// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SHBaseItem.generated.h"

class ASHInteractItem;

UCLASS(BlueprintType)
class PROJECTSH_API USHBaseItem : public UDataAsset
{
	GENERATED_BODY()


public:
	FText GetDisplayName() const { return DisplayName; }
	FText GetDisplayDescription() const { return DisplayDescription; }
	bool GetCanStack() const { return bCanStack; }
	int32 GetMaxStackAmount() const { return MaxStackAmount; }
	int32 GetSize() const { return Size; }
	UTexture2D* GetIcon() const { return Icon; }
	TSoftClassPtr<ASHInteractItem> GetInteractItemClass() const { return InteractItemClass; }

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	virtual FString ToString() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	FText DisplayName = FText::FromString("Item Name");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	FText DisplayDescription = FText::FromString("Item Description");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	bool bCanStack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item", meta = (ClampMin = "1", ClampMax = "400", EditCondition = "bCanStack"))
	int32 MaxStackAmount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item", meta = (ClampMin = "1", ClampMax = "2"))
	int32 Size = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	TSoftClassPtr<ASHInteractItem> InteractItemClass;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
