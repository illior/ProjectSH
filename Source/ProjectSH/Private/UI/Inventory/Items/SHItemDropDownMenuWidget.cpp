// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHItemDropDownMenuWidget.h"
#include "Components/PanelWidget.h"
#include "Components/OverlaySlot.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "InventorySystem/SHWeaponData.h"
#include "InventorySystem/Items/SHBaseItem.h"
#include "InventorySystem/Interfaces/SHUsableItem.h"

void USHItemDropDownMenuWidget::Open(USHItemData* InItemData)
{
	SetVisibility(ESlateVisibility::Visible);

	CurrentIndex = 0;
	InspectButton->SetState(ESHButtonState::Hovered);
	InspectButton->SetVisibility(ESlateVisibility::Visible);
	int32 ButtonsCount = 1;
	
	if (InItemData->IsA<USHWeaponData>())
	{
		EquipButton->SetState(ESHButtonState::Normal);
		EquipButton->SetVisibility(ESlateVisibility::Visible);

		FastAccessButton->SetState(ESHButtonState::Normal);
		FastAccessButton->SetVisibility(ESlateVisibility::Visible);
		ButtonsCount += 2;
	}
	else
	{
		EquipButton->SetState(ESHButtonState::Disabled);
		EquipButton->SetVisibility(ESlateVisibility::Collapsed);

		FastAccessButton->SetState(ESHButtonState::Disabled);
		FastAccessButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	CombineButton->SetState(ESHButtonState::Normal);
	CombineButton->SetVisibility(ESlateVisibility::Visible);
	ButtonsCount++;

	if (InItemData->GetItem()->Implements<USHUsableItem>())
	{
		ISHUsableItem* UsableItem = Cast<ISHUsableItem>(InItemData->GetItem());

		UseButton->SetVisibility(ESlateVisibility::Visible);
		UseButton->SetState(UsableItem->CanUse(GetOwningPlayerPawn()) ? ESHButtonState::Normal : ESHButtonState::Disabled);
		ButtonsCount++;
	}
	else
	{
		UseButton->SetVisibility(ESlateVisibility::Collapsed);
		UseButton->SetState(ESHButtonState::Disabled);
	}

	if (InItemData->CanDrop())
	{
		DropButton->SetVisibility(ESlateVisibility::Visible);
		DropButton->SetState(ESHButtonState::Normal);
		ButtonsCount++;
	}
	else
	{
		DropButton->SetVisibility(ESlateVisibility::Collapsed);
		DropButton->SetState(ESHButtonState::Disabled);
	}

	CalculateHeight(ButtonsCount);
}

void USHItemDropDownMenuWidget::OpenToUse(USHItemData* InItemData)
{
	SetVisibility(ESlateVisibility::Visible);

	CurrentIndex = 0;

	InspectButton->SetState(ESHButtonState::Hovered);
	InspectButton->SetVisibility(ESlateVisibility::Visible);

	CombineButton->SetState(ESHButtonState::Normal);
	CombineButton->SetVisibility(ESlateVisibility::Visible);

	UseButton->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetState(ESHButtonState::Normal);

	EquipButton->SetState(ESHButtonState::Disabled);
	EquipButton->SetVisibility(ESlateVisibility::Collapsed);
	FastAccessButton->SetState(ESHButtonState::Disabled);
	FastAccessButton->SetVisibility(ESlateVisibility::Collapsed);
	DropButton->SetVisibility(ESlateVisibility::Collapsed);
	DropButton->SetState(ESHButtonState::Disabled);

	int32 ButtonsCount = 3;

	CalculateHeight(ButtonsCount);
}

void USHItemDropDownMenuWidget::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);

	for (USHBaseMenuButtonWidget* ButtonWidget : Buttons)
	{
		ButtonWidget->SetState(ESHButtonState::Normal);
	}
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetInspectButtonSignature()
{
	return InspectButton->OnClicked;
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetEquipButtonSignature()
{
	return EquipButton->OnClicked;
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetFastAccessButtonSignature()
{
	return FastAccessButton->OnClicked;
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetCombineButtonSignature()
{
	return CombineButton->OnClicked;
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetUseButtonSignature()
{
	return UseButton->OnClicked;
}

FSHOnButtonClickSignature& USHItemDropDownMenuWidget::GetDropButtonSignature()
{
	return DropButton->OnClicked;
}

void USHItemDropDownMenuWidget::CalculateHeight(int32 ButtonsCount)
{
	float MenuHeight = ButtonHeight * ButtonsCount;

	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(InspectButton->GetParent()->Slot);
	if (OverlaySlot != nullptr)
	{
		FMargin SlotPadding = OverlaySlot->GetPadding();
		MenuHeight += SlotPadding.Bottom + SlotPadding.Top;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetSize(FVector2D(CanvasSlot->GetSize().X, MenuHeight));
	}
}

void USHItemDropDownMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(InspectButton);
	Buttons.Add(InspectButton);
	check(EquipButton);
	Buttons.Add(EquipButton);
	check(FastAccessButton);
	Buttons.Add(FastAccessButton);
	check(CombineButton);
	Buttons.Add(CombineButton);
	check(UseButton);
	Buttons.Add(UseButton);
	check(DropButton);
	Buttons.Add(DropButton);
}
