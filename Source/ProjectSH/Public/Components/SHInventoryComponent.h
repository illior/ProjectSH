// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHCoreTypes.h"
#include "SaveSystem/SHSavableComponent.h"
#include "SHInventoryComponent.generated.h"

class USHCraftingList;
class USHBaseItem;
class USHItemData;

USTRUCT(BlueprintType)
struct FSHItemBytesData
{
	GENERATED_BODY()


public:
	UPROPERTY(SaveGame)
	bool bIsWeapon;

	UPROPERTY(SaveGame)
	TArray<uint8> ByteData;
};

DECLARE_MULTICAST_DELEGATE(FSHOnInventoryInitializedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHOnInventoryItemCreatedSignature, USHItemData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHOnInventoryItemRemovedSignature, USHItemData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHOnInventorySlotsIncrementedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FSHOnInventoryRecordAddedSignature, FSHRecord);

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSH_API USHInventoryComponent : public UActorComponent, public ISHSavableComponent
{
	GENERATED_BODY()


public:
	USHInventoryComponent();

	FSHOnInventoryInitializedSignature OnInventoryInitialized;
	FSHOnInventoryItemCreatedSignature OnItemCreated;
	FSHOnInventoryItemRemovedSignature OnItemRemoved;
	FSHOnInventorySlotsIncrementedSignature OnSlotsIncremented;
	FSHOnInventoryRecordAddedSignature OnRecordAdded;

	virtual FSHComponentSaveData GetSaveData_Implementation() override;
	virtual void LoadFromSaveData_Implementation(FSHComponentSaveData InRecord) override;

	FIntPoint GetMaxSize() const { return MaxSize; };
	int32 GetCurrentSlotsCount() const { return CurrentSlotsCount; };
	ESHSlotState GetSlotState(FIntPoint InPosition) const;

	TIndexedContainerIterator<const TArray<TObjectPtr<USHItemData>, FDefaultAllocator>, TObjectPtr<USHItemData> const, int32> GetItemsIterator() const;
	TIndexedContainerIterator<const TArray<FSHRecord, FDefaultAllocator>, FSHRecord const, int32> GetRecordsIterator() const;

	USHItemData* GetItemByPosition(FIntPoint InPosition) const;

	bool CanSetItemPosition(USHItemData* InItemData, FIntPoint InPostion) const;
	bool CanCombineItems(USHBaseItem* InItem1, USHBaseItem* InItem2) const;
	bool CanCraftItems(USHBaseItem* InItem1, USHBaseItem* InItem2) const;

	bool TryAddItem(USHItemData* InItemData);
	void RemoveItem(USHItemData* InItemData);
	void AddSlots(int32 InCount);
	void AddRecord(FSHRecord InRecord);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Items")
	FIntPoint MaxSize = FIntPoint(4, 5);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "ProjectSH: Items", meta = (ClampMin = "1", ClampMax = "20"))
	int32 CurrentSlotsCount = 8;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "ProjectSH: Items")
	TArray<TObjectPtr<USHItemData>> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Items")
	TObjectPtr<USHCraftingList> CraftingList;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "ProjectSH: Records")
	TArray<FSHRecord> Records;

	UPROPERTY(SaveGame)
	TArray<FSHItemBytesData> ItemsByteData;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void PrintMatrix();

	UFUNCTION()
	void ItemPositionChanged(FIntPoint OldPosition, FIntPoint NewPosition);
	UFUNCTION()
	void ItemDropped(USHItemData* InItemData);

	void SetSlotState(const FIntPoint& InPosition, const ESHSlotState& InState);

	void CombineItems(USHItemData* InItemData1, USHItemData* InItemData2);
	void CraftItems(USHItemData* InItemData1, USHItemData* InItemData2);

	virtual void Initialize();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ESHSlotState** SlotsMatrix;

	void CreateMatrix();
	void DestroyMatrix();

	void SerializeItems();
	void UnserializeItems();
};
