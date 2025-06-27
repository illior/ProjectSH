// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHItemsContainerWidget.h"
#include "Components/SHInventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "UI/Inventory/Items/SHItemWidget.h"
#include "InventorySystem/SHItemData.h"

void USHItemsContainerWidget::SetSlotPadding(FMargin InPadding)
{
	SlotPadding = InPadding;
}

FMargin USHItemsContainerWidget::GetSlotPadding() const
{
	return SlotPadding;
}

void USHItemsContainerWidget::SetItemSelected(FIntPoint InPosition)
{
	for (USHItemWidget* ItemWidget : Items)
	{
		USHItemData* ItemData = ItemWidget->GetItemData();
		if (ItemData != nullptr && ItemData->ContainsPoint(InPosition))
		{
			ItemWidget->SetSelected();
		}
	}
}

void USHItemsContainerWidget::SetItemUnselected(FIntPoint InPosition)
{
	for (USHItemWidget* ItemWidget : Items)
	{
		USHItemData* ItemData = ItemWidget->GetItemData();
		if (ItemData != nullptr && ItemData->ContainsPoint(InPosition))
		{
			ItemWidget->SetUnselected();
		}
	}
}

USHItemData* USHItemsContainerWidget::GetItemDataByPosition(FIntPoint InPosition)
{
	for (USHItemWidget* ItemWidget : Items)
	{
		USHItemData* ItemData = ItemWidget->GetItemData();
		if (ItemData != nullptr && ItemData->ContainsPoint(InPosition))
		{
			return ItemData;
		}
	}

	return nullptr;
}

void USHItemsContainerWidget::CreateItems()
{
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}

	GridPanel->ClearChildren();

	USHInventoryComponent* CharacterInventory = PlayerPawn->FindComponentByClass<USHInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	for (TIndexedContainerIterator It = CharacterInventory->GetItemsIterator(); It; ++It)
	{
		if (*It == nullptr)
		{
			continue;
		}

		USHItemWidget* ItemWidget = CreateWidget<USHItemWidget>(GetOwningPlayer(), ItemClass);
		if (ItemWidget == nullptr)
		{
			continue;
		}

		UGridSlot* GridItem = GridPanel->AddChildToGrid(ItemWidget);
		GridItem->SetPadding(SlotPadding);

		ItemWidget->SetItemData(*It);
		Items.Add(ItemWidget);

		(*It)->OnDropped.AddDynamic(this, &USHItemsContainerWidget::ItemDropped);
	}

	CharacterInventory->OnItemRemoved.AddUObject(this, &USHItemsContainerWidget::ItemDropped);
}

void USHItemsContainerWidget::CreateItem(USHItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		return;
	}

	USHItemWidget* ItemWidget = CreateWidget<USHItemWidget>(GetOwningPlayer(), ItemClass);
	if (ItemWidget == nullptr)
	{
		return;
	}

	UGridSlot* GridItem = GridPanel->AddChildToGrid(ItemWidget);
	GridItem->SetPadding(SlotPadding);

	ItemWidget->SetItemData(InItemData);
	Items.Add(ItemWidget);

	InItemData->OnDropped.AddDynamic(this, &USHItemsContainerWidget::ItemDropped);
}

TArray<USHItemData*> USHItemsContainerWidget::GetAllItems()
{
	TArray<USHItemData*> AllItems;
	for (USHItemWidget* ItemWidget : Items)
	{
		AllItems.Add(ItemWidget->GetItemData());
	}

	return AllItems;
}

void USHItemsContainerWidget::ItemDropped(USHItemData* InItemData)
{
	for (USHItemWidget* ItemWidget : Items)
	{
		if (ItemWidget->GetItemData() == InItemData)
		{
			ItemWidget->RemoveFromParent();
			ItemWidget->MarkAsGarbage();
			Items.Remove(ItemWidget);

			return;
		}
	}
}

void USHItemsContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(GridPanel);
}
