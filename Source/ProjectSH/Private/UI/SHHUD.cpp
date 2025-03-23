// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHHUD.h"
#include "UI/SHKeysTextures.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "EnhancedInputSubsystems.h"

void ASHHUD::OpenInventoryWidget(FSHInventoryPages Page)
{
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

void ASHHUD::OpenPauseWidget()
{
	PauseWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASHHUD::CloseWidgets()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PauseWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

TSoftObjectPtr<UTexture2D> ASHHUD::GetTextureForKey(FKey InKey)
{
	return IsValid(KeysTextures) ? *KeysTextures->KeyToTextureMap.Find(InKey) : TSoftObjectPtr<UTexture2D>();
}

void ASHHUD::BeginPlay()
{
	Super::BeginPlay();

	PauseWidget = CreateWidget<USHBaseInputWidget>(PlayerOwner, PauseWidgetClass, FName(TEXT("PauseWidget")));
	PauseWidget->AddToViewport();
	PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

	InventoryWidget = CreateWidget<USHInventoryWidget>(PlayerOwner, InventoryWidgetClass, FName(TEXT("InventoryWidget")));
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}
