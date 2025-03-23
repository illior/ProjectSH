// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHInputActionDescriptionWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USHInputActionDescriptionWidget::SetKeyTexture(TSoftObjectPtr<UTexture2D> InTexture)
{
	KeyImage->SetBrushFromSoftTexture(InTexture);
}

void USHInputActionDescriptionWidget::SetKeyDescription(FText InText)
{
	if (InText.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	TextDescription->SetText(InText);
}

void USHInputActionDescriptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(KeyImage);
	check(TextDescription);
}
