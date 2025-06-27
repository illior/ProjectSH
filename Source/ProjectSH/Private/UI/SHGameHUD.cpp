// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHGameHUD.h"

#include "UI/Pause/SHPauseWidget.h"
#include "UI/Inventory/SHInventoryWidget.h"

void ASHGameHUD::OpenInventoryWidget(FSHInventoryPages Page)
{
	SetInputWidget(InventoryWidget);

	switch (Page)
	{
	case FSHInventoryPages::Map:
		InventoryWidget->OpenMap();
		break;
	case FSHInventoryPages::Items:
		InventoryWidget->OpenItems();
		break;
	case FSHInventoryPages::Records:
		InventoryWidget->OpenRecords();
		break;
	default:
		InventoryWidget->OpenItems();
		break;
	}
}

void ASHGameHUD::OpenPauseWidget()
{
	SetInputWidget(PauseWidget);
}

void ASHGameHUD::CloseWidgets()
{
	SetInputWidget(nullptr);
}

USHBaseInputWidget* ASHGameHUD::GetPauseWidget() const
{
	return PauseWidget;
}

USHInventoryWidget* ASHGameHUD::GetInventoryWidget() const
{
	return InventoryWidget;
}

void ASHGameHUD::BeginPlay()
{
	Super::BeginPlay();

	PauseWidget = CreateWidget<USHPauseWidget>(PlayerOwner, PauseWidgetClass, FName(TEXT("PauseWidget")));
	PauseWidget->AddToViewport();
	PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

	InventoryWidget = CreateWidget<USHInventoryWidget>(PlayerOwner, InventoryWidgetClass, FName(TEXT("InventoryWidget")));
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}
