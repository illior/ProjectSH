// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHBaseMenuWidget.h"
#include "UI/SHSlideButton.h"
#include "UI/SHBaseMenuButtonWidget.h"

void USHBaseMenuWidget::SetButtonState(int32 Index, ESHButtonState InState)
{
	if (Buttons.IsValidIndex(Index))
	{
		Buttons[Index]->SetState(InState);
	}
}

void USHBaseMenuWidget::SetButtonState(USHBaseMenuButtonWidget* InButton, ESHButtonState InState)
{
	if (Buttons.Find(InButton) != INDEX_NONE)
	{
		InButton->SetState(InState);
	}
}

void USHBaseMenuWidget::SetButtonSelected(USHBaseMenuButtonWidget* InButton)
{
	int32 ButtonIndex = Buttons.Find(InButton);
	if (ButtonIndex != INDEX_NONE && ButtonIndex != CurrentIndex)
	{
		SetButtonState(CurrentIndex, ESHButtonState::Normal);

		CurrentIndex = ButtonIndex;
		InButton->SetState(ESHButtonState::Hovered);
	}
}

void USHBaseMenuWidget::InFocus_Implementation()
{
	SetButtonState(CurrentIndex, ESHButtonState::Hovered);
}

void USHBaseMenuWidget::OutFocus_Implementation()
{
	SetButtonState(CurrentIndex, ESHButtonState::Normal);
	CurrentIndex = 0;
}

void USHBaseMenuWidget::Apply_Implementation()
{
	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->Click();
	}
}

void USHBaseMenuWidget::Cancel_Implementation()
{
	OnCloseMenu.Broadcast();
}

void USHBaseMenuWidget::ResetBind_Implementation()
{

}

void USHBaseMenuWidget::DeleteBind_Implementation()
{

}

void USHBaseMenuWidget::MoveVertical_Implementation(int32 Offset)
{
	int32 NextIndex = FMath::FMath::Clamp(CurrentIndex + Offset, 0, Buttons.Num() - 1);
	if (!Buttons.IsValidIndex(NextIndex) || NextIndex == CurrentIndex)
	{
		return;
	}

	if (Buttons[NextIndex]->GetState() == ESHButtonState::Disabled)
	{
		if (NextIndex != Buttons.Num() - 1 && NextIndex != 0)
		{
			MoveVertical(Offset + 1 * FMath::Sign(Offset));
		}
		return;
	}

	Buttons[CurrentIndex]->SetState(ESHButtonState::Normal);
	CurrentIndex = NextIndex;

	Buttons[CurrentIndex]->SetState(ESHButtonState::Hovered);
}

void USHBaseMenuWidget::MoveHorizontal_Implementation(int32 Offset)
{
	if (Buttons.IsValidIndex(CurrentIndex) && Buttons[CurrentIndex]->Implements<USHSlideButton>())
	{
		ISHSlideButton::Execute_HorizontalMove(Buttons[CurrentIndex], Offset);
	}
}

void USHBaseMenuWidget::AdditiveMoveVertical_Implementation(int32 Offset)
{

}

void USHBaseMenuWidget::AdditiveMoveHorizontal_Implementation(int32 Offset)
{

}

void USHBaseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}
