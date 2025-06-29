// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHBaseMenuButtonWidget.h"

#include "UI/SHBaseMenuWidget.h"
#include "UI/SHSlideButton.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USHBaseMenuButtonWidget::Click()
{
	OnClicked.Broadcast();
}

void USHBaseMenuButtonWidget::SetState(ESHButtonState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	switch (CurrentState)
	{
	case ESHButtonState::Normal:
		ButtonImage->SetBrush(NormalBrush);
		break;
	case ESHButtonState::Hovered:
		ButtonImage->SetBrush(HoveredBrush);
		break;
	case ESHButtonState::Pressed:
		ButtonImage->SetBrush(PressedBrush);
		break;
	case ESHButtonState::Disabled:
		ButtonImage->SetBrush(DisabledBrush);
		break;
	}

	ReceiveStateChanged();
}

ESHButtonState USHBaseMenuButtonWidget::GetState() const
{
	return CurrentState;
}

void USHBaseMenuButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ButtonText != nullptr)
	{
		ButtonText->SetText(Text);
	}

	if (ButtonImage != nullptr)
	{
		ButtonImage->SetBrush(NormalBrush);
	}
}

void USHBaseMenuButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(ButtonImage);
	check(ButtonText);
}

FReply USHBaseMenuButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && CurrentState != ESHButtonState::Disabled)
	{
		SetState(ESHButtonState::Pressed);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply USHBaseMenuButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (GetState() == ESHButtonState::Pressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(GetOuter());
		if (IsValid(MenuWidget))
		{
			MenuWidget->SetButtonSelected(this);
		}

		OnClicked.Broadcast();

		return FReply::Unhandled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void USHBaseMenuButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(GetOuter());
	if (IsValid(MenuWidget) && CurrentState == ESHButtonState::Normal)
	{
		MenuWidget->SetButtonSelected(this);
		return;
	}

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void USHBaseMenuButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}
