// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/Settings/SHKeyButtonWidget.h"
#include "UI/MainMenu/Settings/SHKeyBindButtonWidget.h"
#include "Components/Image.h"
#include "UI/SHHUD.h"

void USHKeyButtonWidget::SetKey(FKey InKey)
{
	CurrentKey = InKey;

	if (!CurrentKey.IsValid())
	{
		KeyImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	KeyImage->SetVisibility(ESlateVisibility::Visible);

	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		ASHHUD* GameHUD = PlayerController->GetHUD<ASHHUD>();
		if (IsValid(GameHUD))
		{
			KeyImage->SetBrushFromSoftTexture(GameHUD->GetTextureForKey(CurrentKey));
		}
	}
}

FKey USHKeyButtonWidget::GetKey() const
{
	return CurrentKey;
}

void USHKeyButtonWidget::SetKeyState(ESHKeyButtonState InState)
{
	switch (InState)
	{
	case ESHKeyButtonState::Calm:
		Background->SetBrushTintColor(Calm);
		break;
	case ESHKeyButtonState::Active:
		Background->SetBrushTintColor(Active);
		break;
	case ESHKeyButtonState::Warning:
		Background->SetBrushTintColor(Warning);
		break;
	default:
		break;
	}
}

void USHKeyButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(KeyImage);
	check(Background);
}

FReply USHKeyButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(GetOuter());
	if (IsValid(BindButton) && BindButton->GetState() != ESHButtonState::Disabled)
	{
		SetState(ESHButtonState::Pressed);

		FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply USHKeyButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(GetOuter());
	if (IsValid(BindButton) && BindButton->GetState() != ESHButtonState::Disabled)
	{
		BindButton->Click();
		SetState(ESHButtonState::Hovered);

		FReply::Unhandled();
	}

	return FReply::Unhandled();
}

void USHKeyButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(GetOuter());
	if (IsValid(BindButton))
	{
		BindButton->SetActiveKey(this);
	}
}
