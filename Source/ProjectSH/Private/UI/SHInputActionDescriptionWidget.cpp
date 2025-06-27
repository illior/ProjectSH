// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHInputActionDescriptionWidget.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "UI/SHHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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

void USHInputActionDescriptionWidget::MappingsRebuilt()
{
	SetTexture(KeyImage, MappingName);
}

void USHInputActionDescriptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(KeyImage);
	check(TextDescription);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	Subsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &USHInputActionDescriptionWidget::MappingsRebuilt);
}

void USHInputActionDescriptionWidget::SetTexture(UImage* InKeyImage, FName InMappingName)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController))
	{
		ASHHUD* GameHUD = PlayerController->GetHUD<ASHHUD>();
		if (IsValid(GameHUD))
		{
			InKeyImage->SetBrushFromSoftTexture(GameHUD->GetTextureForMappingName(InMappingName));
		}
	}
}
