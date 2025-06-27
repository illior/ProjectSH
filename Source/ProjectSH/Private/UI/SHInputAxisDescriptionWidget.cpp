// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHInputAxisDescriptionWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USHInputAxisDescriptionWidget::SetKeyDescription(FText InText)
{
	if (InText.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	TextDescription->SetText(InText);

	VerticalPanel->SetVisibility(ESlateVisibility::Visible);
	KeyImage->SetVisibility(ESlateVisibility::Visible);
	KeyRightImage->SetVisibility(ESlateVisibility::Visible);
}

void USHInputAxisDescriptionWidget::SetKeyDescription(FText InText, bool bAxis)
{
	if (InText.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);
	TextDescription->SetText(InText);

	if (bAxis)
	{
		VerticalPanel->SetVisibility(ESlateVisibility::Visible);
		KeyImage->SetVisibility(ESlateVisibility::Collapsed);
		KeyRightImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		VerticalPanel->SetVisibility(ESlateVisibility::Collapsed);
		KeyImage->SetVisibility(ESlateVisibility::Visible);
		KeyRightImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void USHInputAxisDescriptionWidget::MappingsRebuilt()
{
	Super::MappingsRebuilt();

	SetTexture(KeyTopImage, TopMappingName);
	SetTexture(KeyRightImage, RightMappingName);
	SetTexture(KeyBottomImage, BottomMappingName);
}

void USHInputAxisDescriptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(KeyTopImage);
	check(KeyRightImage);
	check(KeyBottomImage);
}
