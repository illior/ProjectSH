// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHSlotWidget.h"
#include "Components/Image.h"

void USHSlotWidget::SetSlotState(ESHSlotState InState)
{
	if (InState == ESHSlotState::Void)
	{
		SetVisibility(ESlateVisibility::Visible);
		if (SlotState == ESHSlotState::Block)
		{
			PlayAnimation(Appearance);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}

	SlotState = InState;
}

void USHSlotWidget::SetSelected()
{
	SelectTexture->SetBrushTintColor(SelectedTint);
}

void USHSlotWidget::SetUnselected()
{
	SelectTexture->SetBrushTintColor(UnselectedTint);
}

void USHSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(SelectTexture);
}
