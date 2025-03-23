// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHItemWidget.h"
#include "InventorySystem/SHWeaponData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/GridSlot.h"

void USHItemWidget::SetItemData(USHItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		return;
	}

	ItemData = TWeakObjectPtr<USHItemData>(InItemData);

	ItemData->OnPositionChanged.AddDynamic(this, &USHItemWidget::PositionChanged);
	ItemData->OnCountChanged.AddDynamic(this, &USHItemWidget::CountChanged);

	UpdateData();
	UpdatePosition();
}

USHItemData* USHItemWidget::GetItemData() const
{
	return ItemData.Get();
}

void USHItemWidget::SetSelected()
{
	SelectTexture->SetBrushTintColor(SelectedTint);
}

void USHItemWidget::SetUnselected()
{
	SelectTexture->SetBrushTintColor(UnselectedTint);
}

void USHItemWidget::PositionChanged(FIntPoint OldPosition, FIntPoint NewPosition)
{
	UGridSlot* GridSlot = Cast<UGridSlot>(Slot);
	if (GridSlot != nullptr && ItemData.IsValid())
	{
		GridSlot->SetRow(NewPosition.Y);
		GridSlot->SetColumn(NewPosition.X);

		GridSlot->SetRowSpan(1);
		GridSlot->SetColumnSpan(ItemData->GetItemSize());
	}
}

void USHItemWidget::CountChanged(int32 NewCount)
{
	CountText->SetText(ItemData->GetItemCanStack() ? FText::FromString(FString::FromInt(ItemData->GetCount())) : FText());
}

void USHItemWidget::UpdateData()
{
	if (!ItemData.IsValid())
	{
		return;
	}

	ItemTexture->SetBrushFromTexture(ItemData->GetItemIcon());

	if (ItemData->IsA<USHWeaponData>())
	{
		USHWeaponData* WeaponData = Cast<USHWeaponData>(ItemData);
		CountText->SetText(FText::FromString(FString::FromInt(WeaponData->GetCurrentAmmoInClip())));

		EquipText->SetText(WeaponData->GetIsEquip() ? FText::FromString(TEXT("E")) : FText());
		FastAccessText->SetText(WeaponData->GetFastAccessIndex() < 0 ? FText() : FText::FromString(FString::FromInt(WeaponData->GetFastAccessIndex() + 1)));
	}
	else
	{
		CountText->SetText(ItemData->GetItemCanStack() ? FText::FromString(FString::FromInt(ItemData->GetCount())) : FText());

		EquipText->SetText(FText());
		FastAccessText->SetText(FText());
	}
}

void USHItemWidget::UpdatePosition()
{
	UGridSlot* GridSlot = Cast<UGridSlot>(Slot);
	if (GridSlot != nullptr && ItemData.IsValid())
	{
		GridSlot->SetRow(ItemData->GetPosition().Y);
		GridSlot->SetColumn(ItemData->GetPosition().X);

		GridSlot->SetRowSpan(1);
		GridSlot->SetColumnSpan(ItemData->GetItemSize());
	}
}

void USHItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(ItemTexture);
	check(CountText);
	check(EquipText);
	check(FastAccessText);
	check(SelectTexture);
}
