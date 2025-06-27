// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHHUD.h"
#include "UI/SHBaseInputWidget.h"
#include "UI/SHKeysTextures.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

TSoftObjectPtr<UTexture2D> ASHHUD::GetTextureForKey(FKey InKey) const
{
	if (!IsValid(KeysTextures))
	{
		return TSoftObjectPtr<UTexture2D>();
	}

	TSoftObjectPtr<UTexture2D>* TexturePointer = KeysTextures->KeyToTextureMap.Find(InKey);
	if (TexturePointer != nullptr)
	{
		return *TexturePointer;
	}

	return TSoftObjectPtr<UTexture2D>();
}

TSoftObjectPtr<UTexture2D> ASHHUD::GetTextureForMappingName(FName InMappingName) const
{
	if (!IsValid(PlayerOwner))
	{
		return TSoftObjectPtr<UTexture2D>();
	}

	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = PlayerOwner->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!IsValid(EISubsystem))
	{
		return TSoftObjectPtr<UTexture2D>();
	}

	const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
	if (!IsValid(UserSettings))
	{
		return TSoftObjectPtr<UTexture2D>();
	}

	UEnhancedPlayerMappableKeyProfile* KeyProfile = UserSettings->GetCurrentKeyProfile();
	if (!IsValid(KeyProfile))
	{
		return TSoftObjectPtr<UTexture2D>();
	}

	FMapPlayerKeyArgs Args = {};
	Args.MappingName = InMappingName;
	Args.Slot = EPlayerMappableKeySlot::First;

	FPlayerKeyMapping* KeyMapping = KeyProfile->FindKeyMapping(Args);
	if (KeyMapping != nullptr)
	{
		return GetTextureForKey(KeyMapping->GetCurrentKey());
	}

	return TSoftObjectPtr<UTexture2D>();
}

void ASHHUD::SetInputWidget(USHBaseInputWidget* InWidget)
{
	if (IsValid(CurrentInputWidget))
	{
		CurrentInputWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(InWidget))
	{
		InWidget->SetVisibility(ESlateVisibility::Visible);
	}

	CurrentInputWidget = InWidget;
}

USHBaseInputWidget* ASHHUD::GetCurrentInputWidget() const
{
	return CurrentInputWidget;
}

void ASHHUD::BeginPlay()
{
	Super::BeginPlay();

}
