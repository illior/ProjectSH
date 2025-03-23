// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHSlotsContainerWidget.h"
#include "Components/SHInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/Inventory/Items/SHSlotWidget.h"

FIntPoint USHSlotsContainerWidget::GetInventorySize() const
{
	return InventorySize;
}

int32 USHSlotsContainerWidget::GetCurrentSlotsCount() const
{
	return CurrentSlotsCount;
}

void USHSlotsContainerWidget::SetSlotPadding(FMargin InPadding)
{
	SlotPadding = InPadding;
}

FMargin USHSlotsContainerWidget::GetSlotPadding() const
{
	return SlotPadding;
}

ESHSlotState USHSlotsContainerWidget::GetSlotState(FIntPoint InPosition) const
{
	if (InPosition.Y < 0 || InPosition.Y >= InventorySize.Y ||
		InPosition.X < 0 || InPosition.X >= InventorySize.X)
	{
		return ESHSlotState::Block;
	}

	int Index = InPosition.Y * InventorySize.X + InPosition.X;

	return Slots[Index]->GetSlotState();
}

bool USHSlotsContainerWidget::CanClose() const
{
	return !Slots[CurrentSlotsCount - 1]->IsAnyAnimationPlaying();
}

void USHSlotsContainerWidget::SetSlotSelected(FIntPoint InPosition)
{
	if (InPosition.Y < 0 || InPosition.Y >= InventorySize.Y ||
		InPosition.X < 0 || InPosition.X >= InventorySize.X)
	{
		return;
	}

	int Index = InPosition.Y * InventorySize.X + InPosition.X;
	Slots[Index]->SetSelected();
}

void USHSlotsContainerWidget::SetSlotUnselected(FIntPoint InPosition)
{
	if (InPosition.Y < 0 || InPosition.Y >= InventorySize.Y ||
		InPosition.X < 0 || InPosition.X >= InventorySize.X)
	{
		return;
	}

	int Index = InPosition.Y * InventorySize.X + InPosition.X;
	Slots[Index]->SetUnselected();
}

void USHSlotsContainerWidget::CreateSlots()
{
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}

	USHInventoryComponent* CharacterInventory = PlayerPawn->GetComponentByClass<USHInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	InventorySize = CharacterInventory->GetMaxSize();
	CurrentSlotsCount = CharacterInventory->GetCurrentSlotsCount();

	GridPanel->ClearChildren();
	Slots.Empty();

	for (int32 Y = 0; Y < InventorySize.Y; Y++)
	{
		for (int32 X = 0; X < InventorySize.X; X++)
		{
			USHSlotWidget* SlotWidget = CreateWidget<USHSlotWidget>(GetOwningPlayer(), SlotClass);
			if (SlotWidget == nullptr)
			{
				continue;
			}

			SlotWidget->SetSlotState(CharacterInventory->GetSlotState(FIntPoint(X, Y)));
			Slots.Add(SlotWidget);

			UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(SlotWidget, Y, X);
			GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}
}

void USHSlotsContainerWidget::UpdateSlots()
{
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}

	USHInventoryComponent* CharacterInventory = PlayerPawn->GetComponentByClass<USHInventoryComponent>();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	CurrentSlotsCount = CharacterInventory->GetCurrentSlotsCount();

	for (int32 Y = 0; Y < InventorySize.Y; Y++)
	{
		for (int32 X = 0; X < InventorySize.X; X++)
		{
			int32 Index = X + Y * InventorySize.X;
			Slots[Index]->SetSlotState(CharacterInventory->GetSlotState(FIntPoint(X, Y)));
		}
	}
}

void USHSlotsContainerWidget::AddSlots(int32 InCount)
{
	for (int32 i = CurrentSlotsCount; i < CurrentSlotsCount + InCount; i++)
	{
		Slots[i]->SetSlotState(ESHSlotState::Void);
	}

	CurrentSlotsCount += InCount;
}

void USHSlotsContainerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (GridPanel != nullptr)
	{
		GridPanel->SetSlotPadding(SlotPadding);
	}
}

void USHSlotsContainerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(GridPanel);
}
