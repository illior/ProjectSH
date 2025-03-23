// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHItemsContainerWidget.generated.h"

class UGridPanel;
class USHItemWidget;

class USHItemData;

UCLASS()
class PROJECTSH_API USHItemsContainerWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetSlotPadding(FMargin InPadding);
	FMargin GetSlotPadding() const;

	void SetItemSelected(FIntPoint InPosition);
	void SetItemUnselected(FIntPoint InPosition);

	USHItemData* GetItemDataByPosition(FIntPoint InPosition);

	void CreateItems();
	void CreateItem(USHItemData* InItemData);

	TArray<USHItemData*> GetAllItems();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	TSubclassOf<USHItemWidget> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	FMargin SlotPadding = FMargin();

	TArray<TObjectPtr<USHItemWidget>> Items;

	UFUNCTION()
	void ItemDropped(USHItemData* InItemData);

	virtual void NativeOnInitialized() override;
};
