// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/Settings/SHKeyBindButtonWidget.h"
#include "UI/MainMenu/Settings/SHKeyButtonWidget.h"
#include "Components/TextBlock.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "UI/MainMenu/Settings/SHPlayerMappableKeySettings.h"

void USHKeyBindButtonWidget::UpdateData()
{
	UEnhancedPlayerMappableKeyProfile* CurrentKeyProfile = GetCurrentKeyProfile();
	if (!IsValid(CurrentKeyProfile))
	{
		return;
	}

	FMapPlayerKeyArgs Args = {};
	Args.MappingName = MappingName;
	Args.Slot = EPlayerMappableKeySlot::First;

	FPlayerKeyMapping* KeyMapping = CurrentKeyProfile->FindKeyMapping(Args);
	if (KeyMapping != nullptr)
	{
		Text = KeyMapping->GetDisplayName();
		ButtonText->SetText(Text);

		FirstKeyButton->SetKey(KeyMapping->GetCurrentKey());
	}
	else
	{
		FirstKeyButton->SetKey(FKey());
	}

	Args.Slot = EPlayerMappableKeySlot::Second;
	KeyMapping = CurrentKeyProfile->FindKeyMapping(Args);
	if (KeyMapping != nullptr)
	{
		SecondKeyButton->SetKey(KeyMapping->GetCurrentKey());
	}
	else
	{
		SecondKeyButton->SetKey(FKey());
	}

	MappingCategory = GetCurrentMappingCategory();

	SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Calm);
	SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Calm);
}

void USHKeyBindButtonWidget::SetActiveKey(USHKeyButtonWidget* InKeyButton)
{
	if (CurrentState == ESHButtonState::Disabled)
	{
		return;
	}

	if (InKeyButton == FirstKeyButton)
	{
		FirstKeyButton->SetState(ESHButtonState::Hovered);
		SecondKeyButton->SetState(ESHButtonState::Normal);

		ActiveSlot = EPlayerMappableKeySlot::First;
	}
	else if (InKeyButton == SecondKeyButton)
	{
		FirstKeyButton->SetState(ESHButtonState::Normal);
		SecondKeyButton->SetState(ESHButtonState::Hovered);

		ActiveSlot = EPlayerMappableKeySlot::Second;
	}
}

void USHKeyBindButtonWidget::SetKeyState(EPlayerMappableKeySlot KeySlot, ESHKeyButtonState KeyButtonState)
{
	switch (KeySlot)
	{
	case EPlayerMappableKeySlot::First:
		FirstKeyButton->SetKeyState(KeyButtonState);
		break;
	case EPlayerMappableKeySlot::Second:
		SecondKeyButton->SetKeyState(KeyButtonState);
		break;
	}
}

void USHKeyBindButtonWidget::BindKey(EPlayerMappableKeySlot KeySlot, FKey InKey)
{
	switch (KeySlot)
	{
	case EPlayerMappableKeySlot::First:
		FirstKeyButton->SetKey(InKey);
		break;
	case EPlayerMappableKeySlot::Second:
		SecondKeyButton->SetKey(InKey);
		break;
	}
}

void USHKeyBindButtonWidget::ResetKey(EPlayerMappableKeySlot KeySlot)
{
	UEnhancedPlayerMappableKeyProfile* CurrentKeyProfile = GetCurrentKeyProfile();
	if (CurrentKeyProfile == nullptr)
	{
		return;
	}

	FMapPlayerKeyArgs Args = {};
	Args.MappingName = MappingName;
	Args.Slot = KeySlot;

	FPlayerKeyMapping* KeyMapping = CurrentKeyProfile->FindKeyMapping(Args);
	if (KeyMapping != nullptr)
	{
		BindKey(KeySlot, KeyMapping->GetDefaultKey());
	}
}

void USHKeyBindButtonWidget::DeleteKey(EPlayerMappableKeySlot KeySlot)
{
	BindKey(KeySlot, FKey());
}

void USHKeyBindButtonWidget::SaveBindings()
{
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(EISubsystem))
	{
		return;
	}

	UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
	if (!IsValid(UserSettings))
	{
		return;
	}

	FGameplayTagContainer FailureReason;

	FMapPlayerKeyArgs Args = {};
	Args.MappingName = MappingName;

	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = GetKeyForSlot(Args.Slot);
	UserSettings->MapPlayerKey(Args, FailureReason);

	Args.Slot = EPlayerMappableKeySlot::Second;
	Args.NewKey = GetKeyForSlot(Args.Slot);
	UserSettings->MapPlayerKey(Args, FailureReason);
}

FKey USHKeyBindButtonWidget::GetKeyForSlot(EPlayerMappableKeySlot KeySlot) const
{
	switch (KeySlot)
	{
	case EPlayerMappableKeySlot::First:
		return FirstKeyButton->GetKey();
		break;
	case EPlayerMappableKeySlot::Second:
		return SecondKeyButton->GetKey();
		break;
	default:
		return FKey();
		break;
	}
}

ESHMappableKeyCategory USHKeyBindButtonWidget::GetMappingCategory() const
{
	return MappingCategory;
}

void USHKeyBindButtonWidget::SetState(ESHButtonState NewState)
{
	Super::SetState(NewState);

	if (NewState == ESHButtonState::Normal || NewState == ESHButtonState::Disabled)
	{
		FirstKeyButton->SetState(ESHButtonState::Normal);
		SecondKeyButton->SetState(ESHButtonState::Normal);


		ActiveSlot = EPlayerMappableKeySlot::First;
	}
	else
	{
		switch (ActiveSlot)
		{
		case EPlayerMappableKeySlot::First:
			FirstKeyButton->SetState(ESHButtonState::Hovered);
			SecondKeyButton->SetState(ESHButtonState::Normal);
			break;
		case EPlayerMappableKeySlot::Second:
			FirstKeyButton->SetState(ESHButtonState::Normal);
			SecondKeyButton->SetState(ESHButtonState::Hovered);
			break;
		}
	}
}

void USHKeyBindButtonWidget::HorizontalMove_Implementation(int32 Offset)
{
	if (Offset < 0)
	{
		FirstKeyButton->SetState(ESHButtonState::Hovered);
		SecondKeyButton->SetState(ESHButtonState::Normal);

		ActiveSlot = EPlayerMappableKeySlot::First;
	}
	else
	{
		SecondKeyButton->SetState(ESHButtonState::Hovered);
		FirstKeyButton->SetState(ESHButtonState::Normal);

		ActiveSlot = EPlayerMappableKeySlot::Second;
	}
}

void USHKeyBindButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(FirstKeyButton);
	check(SecondKeyButton);

	FirstKeyButton->Rename(nullptr, this);
	SecondKeyButton->Rename(nullptr, this);
}

UEnhancedPlayerMappableKeyProfile* USHKeyBindButtonWidget::GetCurrentKeyProfile() const
{
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (IsValid(EISubsystem))
	{
		const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
		if (IsValid(UserSettings))
		{
			return UserSettings->GetCurrentKeyProfile();
		}
	}

	return nullptr;
}

ESHMappableKeyCategory USHKeyBindButtonWidget::GetCurrentMappingCategory() const
{
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(EISubsystem))
	{
		return ESHMappableKeyCategory();
	}

	const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
	if (!IsValid(UserSettings))
	{
		return ESHMappableKeyCategory();
	}

	TSet<TObjectPtr<const UInputMappingContext>> RegisteredInputMappingContexts = UserSettings->GetRegisteredInputMappingContexts();

	for (TObjectPtr<const UInputMappingContext> RegisteredInputMappingContext : RegisteredInputMappingContexts)
	{
		const TArray<FEnhancedActionKeyMapping> Mappings = RegisteredInputMappingContext->GetMappings();
		for (FEnhancedActionKeyMapping Mapping : Mappings)
		{
			USHPlayerMappableKeySettings* MappableKeySettings = Mapping.GetPlayerMappableKeySettings<USHPlayerMappableKeySettings>();
			if (IsValid(MappableKeySettings) && MappableKeySettings->Name == MappingName)
			{
				return MappableKeySettings->Category;
			}
		}
	}

	return ESHMappableKeyCategory();
}
