// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SHSubInventoryWidget.h"
#include "SHCoreTypes.h"
#include "SHItemsWidget.generated.h"

class UTextBlock;
class USHSlotsContainerWidget;
class USHItemsContainerWidget;
class USHItemsCursorWidget;
class USHItemDropDownMenuWidget;
class USHItemData;

UCLASS()
class PROJECTSH_API USHItemsWidget : public USHSubInventoryWidget
{
	GENERATED_BODY()


public:
	void TryAddItem(USHItemData* InItemData);

	virtual void Show(bool bShowAnim) override;
	virtual void Hide(bool bShowAnim) override;

	virtual void Apply() override;
	virtual void Cancel() override;

	virtual void MoveWithFrequency(FVector2D Value) override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHSlotsContainerWidget> SlotsContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHItemsContainerWidget> ItemsContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHItemsCursorWidget> ItemsCursor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHItemDropDownMenuWidget> DropDownMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FMargin SlotPadding = FMargin(0.0f, 0.0f, 10.0f, 10.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FVector2D SlotSize = FVector2D(100.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText ApplyDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText CancelDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText CloseDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText MoveDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText VerticalDescription;

	UPROPERTY()
	TWeakObjectPtr<USHItemData> MovingItem;

	ESHInventoryOpenState OpenState = ESHInventoryOpenState::Default;
	ESHItemInventoryState CurrentState = ESHItemInventoryState::Select;

	UFUNCTION()
	void InspectItem();
	UFUNCTION()
	void EquipItem();
	UFUNCTION()
	void FastAccessItem();
	UFUNCTION()
	void CombineItem();
	void CancelMovingItem();
	void StopMovingItem();
	UFUNCTION()
	void UseItem();
	UFUNCTION()
	void DropItem();

	void TryCloseAddSlots();

	void InventoryInitialized();
	void ItemCreated(USHItemData* InItemData);
	void SlotsIncremented(int32 InCount);

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	FIntPoint GetBestPosition(USHItemData* InItemData);

	void RemoveCurrentItem();

	void OpenDropDownMenu();
	void CloseDropDownMenu();

	void MoveCursor(FIntPoint Offset);

	void SetCurrentSelected();
	void SetCurrentUnselected();
};
