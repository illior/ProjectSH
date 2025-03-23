// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHBaseMenuWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"

void USHBaseMenuWidget::Apply()
{
	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->Click();
	}
}

void USHBaseMenuWidget::MoveVertical(int32 Offset)
{
	int32 NextIndex = FMath::FMath::Clamp(CurrentIndex + Offset, 0, Buttons.Num() - 1);

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

void USHBaseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}
