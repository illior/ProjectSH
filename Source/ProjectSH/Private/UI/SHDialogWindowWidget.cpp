// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHDialogWindowWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "Components/TextBlock.h"

void USHDialogWindowWidget::InFocus_Implementation()
{
	SetVisibility(ESlateVisibility::Visible);

	Super::InFocus_Implementation();
}

void USHDialogWindowWidget::OutFocus_Implementation()
{
	Super::OutFocus_Implementation();

	SetVisibility(ESlateVisibility::Collapsed);
}

void USHDialogWindowWidget::SetDescription(FText InText)
{
	ContentText->SetText(InText);
}

void USHDialogWindowWidget::PositiveButtonHandle()
{
	DialogWindowResponse.Broadcast(true);
}

void USHDialogWindowWidget::NegativeButtonHandle()
{
	DialogWindowResponse.Broadcast(false);
}

void USHDialogWindowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(PositiveButton);
	check(NegativeButton);
	check(ContentText);

	PositiveButton->OnClicked.AddDynamic(this, &USHDialogWindowWidget::PositiveButtonHandle);
	PositiveButton->Rename(nullptr, this);
	Buttons.Add(PositiveButton);

	NegativeButton->OnClicked.AddDynamic(this, &USHDialogWindowWidget::NegativeButtonHandle);
	NegativeButton->Rename(nullptr, this);
	Buttons.Add(NegativeButton);
}
