// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHItemsCursorWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "InventorySystem/SHItemData.h"

void USHItemsCursorWidget::SetSlotPadding(FMargin InPadding)
{
	SlotPadding = InPadding;
}

FMargin USHItemsCursorWidget::GetSlotPadding() const
{
	return SlotPadding;
}

FIntPoint USHItemsCursorWidget::GetPosition() const
{
	return Position;
}

int32 USHItemsCursorWidget::GetSizeX() const
{
	return TempItem.IsValid() ? TempItem->GetItemSize() : 1;
}

void USHItemsCursorWidget::SetActive(bool bActive)
{
	if (bActive)
	{
		PlayAnimation(Active, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, true);
	}
	else
	{
		StopAnimation(Active);
	}
}

void USHItemsCursorWidget::SetPosition(FIntPoint InPosition, bool bBlend, ESHSlotState RightSlotState)
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);

	if (CanvasSlot != nullptr)
	{
		TargetPosition = FVector2D(
			(CanvasSlot->GetSize().X + (SlotPadding.Left + SlotPadding.Right)) * InPosition.X,
			(CanvasSlot->GetSize().Y + (SlotPadding.Top + SlotPadding.Bottom)) * InPosition.Y
		);

		bShouldMove = bBlend;
		if (bBlend)
		{
			FIntPoint DeltaPosition = Position - InPosition;
			FVector2D DeltaTranslation = CursorBox->GetRenderTransform().Translation + FVector2D(
				DeltaPosition.X * (CanvasSlot->GetSize().X + SlotPadding.Left + SlotPadding.Right),
				DeltaPosition.Y * (CanvasSlot->GetSize().X + SlotPadding.Top + SlotPadding.Bottom)
			);

			DeltaTranslation.X = FMath::Clamp(DeltaTranslation.X, -CanvasSlot->GetSize().X - SlotPadding.Left - SlotPadding.Right, CanvasSlot->GetSize().X + SlotPadding.Left + SlotPadding.Right);
			DeltaTranslation.Y = FMath::Clamp(DeltaTranslation.Y, -CanvasSlot->GetSize().Y - SlotPadding.Top - SlotPadding.Bottom, CanvasSlot->GetSize().Y + SlotPadding.Top + SlotPadding.Bottom);

			CursorBox->SetRenderTranslation(DeltaTranslation);
		}

		CanvasSlot->SetPosition(TargetPosition);
	}

	Position = InPosition;
	if (TempItem.IsValid())
	{
		if (TempItem->GetItemSize() == 2)
		{
			if (RightSlotState == ESHSlotState::Block)
			{
				TempItem->SetPosition(Position - FIntPoint(1, 0));

				UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ItemImage->GetParent()->Slot);
				if (OverlaySlot != nullptr && CanvasSlot != nullptr)
				{
					OverlaySlot->SetPadding(FMargin(-CanvasSlot->GetSize().X - SlotPadding.Right - SlotPadding.Left, 0, 0, 0));
				}
			}
			else
			{
				TempItem->SetPosition(Position);

				UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ItemImage->GetParent()->Slot);
				if (OverlaySlot != nullptr && CanvasSlot != nullptr)
				{
					OverlaySlot->SetPadding(FMargin(0, 0, -CanvasSlot->GetSize().X - SlotPadding.Right - SlotPadding.Left, 0));
				}
			}
		}
		else
		{
			TempItem->SetPosition(Position);
		}
	}
}

void USHItemsCursorWidget::SetTempItem(USHItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		return;
	}

	ItemImage->SetVisibility(ESlateVisibility::Visible);
	ItemText->SetVisibility(ESlateVisibility::Visible);

	TempItem = InItemData;
	TempItem->OnCountChanged.AddDynamic(this, &USHItemsCursorWidget::CountChanged);

	ItemImage->SetBrushFromSoftTexture(TempItem->GetItemIcon());

	if (TempItem->GetItemCanStack())
	{
		ItemText->SetText(FText::FromString(FString::FromInt(TempItem->GetCount())));
	}
	else
	{
		ItemText->SetText(FText());
	}

	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ItemImage->GetParent()->Slot);
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	if (OverlaySlot != nullptr && CanvasSlot != nullptr)
	{
		if (TempItem->GetItemSize() == 1)
		{
			OverlaySlot->SetPadding(FMargin(0, 0, 0, 0));
		}
		else
		{
			OverlaySlot->SetPadding(FMargin(0, 0, -CanvasSlot->GetSize().X - SlotPadding.Right - SlotPadding.Left, 0));
		}
	}
}

USHItemData* USHItemsCursorWidget::GetTempItem() const
{
	return TempItem.Get();
}

void USHItemsCursorWidget::ClearTempItem()
{
	ItemImage->SetVisibility(ESlateVisibility::Hidden);
	ItemText->SetVisibility(ESlateVisibility::Hidden);

	if (TempItem.IsValid())
	{
		TempItem->OnCountChanged.RemoveDynamic(this, &USHItemsCursorWidget::CountChanged);
		TempItem.Reset();
	}

	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(ItemImage->GetParent()->Slot);
	if (OverlaySlot != nullptr)
	{
		OverlaySlot->SetPadding(FMargin(0, 0, 0, 0));
	}
}

void USHItemsCursorWidget::CountChanged(int32 NewCount)
{
	if (TempItem.IsValid())
	{
		ItemText->SetText(FText::FromString(FString::FromInt(TempItem->GetCount())));
	}
}

void USHItemsCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bShouldMove)
	{
		FVector2D CurrentPosition = CursorBox->GetRenderTransform().Translation;
		if (CurrentPosition.Equals(FVector2D::ZeroVector, 0.5 + Speed * InDeltaTime))
		{
			CursorBox->SetRenderTranslation(FVector2D::ZeroVector);
			bShouldMove = false;
		}
		else
		{
			FVector2D NewPosition = FVector2D(
				CurrentPosition.X - FMath::Sign(CurrentPosition.X) * Speed * InDeltaTime,
				CurrentPosition.Y - FMath::Sign(CurrentPosition.Y) * Speed * InDeltaTime
			);

			CursorBox->SetRenderTranslation(NewPosition);
		}
	}
}
