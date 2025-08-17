// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SHItemData.generated.h"

class USHBaseItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSHOnItemPositionChangedSignature, FIntPoint, OldPosition, FIntPoint, NewPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHOnItemCountChangedSignature, int32, NewCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHOnItemDroppedSignature, USHItemData*, DroppedItem);

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class PROJECTSH_API USHItemData : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Category = "InventorySystem")
	FGameplayTagContainer ItemTags;

	FSHOnItemPositionChangedSignature OnPositionChanged;
	FSHOnItemCountChangedSignature OnCountChanged;
	FSHOnItemDroppedSignature OnDropped;

	template<class T>
	T* GetItem() const { return  Cast<T>(Item); };
	USHBaseItem* GetItem() const { return Item; };
	int32 GetCount() const { return Count; };
	FIntPoint GetPosition() const { return Position; };

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	FText GetItemName() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	FText GetItemDescription() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool GetItemCanStack() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemMaxStackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemSize() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	UTexture2D* GetItemIcon() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	TSoftClassPtr<ASHInteractItem> GetInteractItemClass() const;

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	virtual FString ToString() const;

	virtual void Initialize(USHBaseItem* InItem, int32 InCount, FIntPoint InPosition);

	bool SetPosition(FIntPoint InPosition);
	void SetCount(int32 InCount);
	void DropItem();

	bool ContainsPoint(FIntPoint InPosition) const;
	virtual bool IsFull() const;
	virtual bool CanDrop() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	TObjectPtr<USHBaseItem> Item = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem", meta = (ClampMin = "1"))
	int32 Count = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	FIntPoint Position = FIntPoint(0, 0);

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
