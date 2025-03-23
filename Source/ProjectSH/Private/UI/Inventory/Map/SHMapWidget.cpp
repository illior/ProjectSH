// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Map/SHMapWidget.h"
#include "UI/Inventory/SHInventoryWidget.h"

void USHMapWidget::Show(bool bShowAnim)
{
	Super::Show(bShowAnim);

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetApplyDescription(FText());
		InventoryWidget->SetCancelDescription(FText());

		InventoryWidget->SetMoveDescription(FText());
	}
}

void USHMapWidget::Apply()
{
	UE_LOG(LogTemp, Display, TEXT("Map Apply"));
}

void USHMapWidget::Cancel()
{
	UE_LOG(LogTemp, Display, TEXT("Map Cancel"));
	Close();
}

void USHMapWidget::MoveWithFrequency(FVector2D Value)
{
	UE_LOG(LogTemp, Display, TEXT("Map Move %s"), *Value.ToString());
}
