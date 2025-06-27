// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Pause/SHPauseWidget.h"

#include "Components/WidgetSwitcher.h"
#include "UI/SHBaseMenuWidget.h"
#include "UI/SHDialogWindowWidget.h"

void USHPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(PauseMenu);
	check(ReturnDialog);
}

void USHPauseWidget::Move(FVector2D Value, float ElapsedTime)
{
	USHBaseMenuWidget* MenuWidget;
	if (ReturnDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		MenuWidget = ReturnDialog;
	}
	else
	{
		MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	}

	if (!IsValid(MenuWidget))
	{
		return;
	}

	float DynamicFrequency = FMath::Max(MinFrequency, Frequency - ElapsedTime * FrequencyDecayRate);

	if (Value.Y != 0.0f && ElapsedTime > FrequencyMoveTime.Y)
	{
		FrequencyMoveTime.Y = ElapsedTime + DynamicFrequency;

		MenuWidget->MoveVertical(-Value.Y);
	}

	if (Value.X != 0.0f && ElapsedTime > FrequencyMoveTime.X)
	{
		FrequencyMoveTime.X = ElapsedTime + DynamicFrequency;

		MenuWidget->MoveHorizontal(Value.X);
	}
}

void USHPauseWidget::Apply()
{
	USHBaseMenuWidget* MenuWidget;
	if (ReturnDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		MenuWidget = ReturnDialog;
	}
	else
	{
		MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	}

	if (IsValid(MenuWidget))
	{
		MenuWidget->Apply();
	}
}

void USHPauseWidget::Cancel()
{
	if (ReturnDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		ReturnDialog->Cancel();
		return;
	}

	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (IsValid(MenuWidget) && MenuWidget != PauseMenu)
	{
		MenuWidget->Cancel();
		return;
	}

	Close();
}

void USHPauseWidget::ResetBind()
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (IsValid(MenuWidget))
	{
		MenuWidget->ResetBind();
	}
}

void USHPauseWidget::DeleteBind()
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (IsValid(MenuWidget))
	{
		MenuWidget->DeleteBind();
	}
}
