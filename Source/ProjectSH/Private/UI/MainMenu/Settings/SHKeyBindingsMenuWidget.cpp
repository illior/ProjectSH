// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/Settings/SHKeyBindingsMenuWidget.h"
#include "UI/MainMenu/Settings/SHKeyBindButtonWidget.h"
#include "UI/SHDialogWindowWidget.h"
#include "Components/ScrollBox.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogKeyBindings);

void USHKeyBindingsMenuWidget::SetButtonSelected(USHBaseMenuButtonWidget* InButton)
{
	Super::SetButtonSelected(InButton);

	if (CurrentIndex == 0)
	{
		ScrollBox->ScrollToStart();
	}
	else if (CurrentIndex == Buttons.Num() - 1)
	{
		ScrollBox->ScrollToEnd();
	}
	else
	{
		ScrollBox->ScrollWidgetIntoView(Buttons[CurrentIndex]);
	}
}

void USHKeyBindingsMenuWidget::InFocus_Implementation()
{
	if (Buttons.IsValidIndex(CurrentIndex))
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
		if (IsValid(BindButton))
		{
			BindButton->ActiveSlot = EPlayerMappableKeySlot::First;

			BindButton->SetState(ESHButtonState::Hovered);
		}
	}

	SetDescription();
}

void USHKeyBindingsMenuWidget::OutFocus_Implementation()
{
	Super::OutFocus_Implementation();

	ScrollBox->ScrollToStart();
}

void USHKeyBindingsMenuWidget::Apply_Implementation()
{
	if (bListenKey)
	{
		return;
	}

	if (IsDialogWindowActive())
	{
		DialogWindow->Apply();
	}
	else
	{
		Super::Apply_Implementation();
	}
}

void USHKeyBindingsMenuWidget::Cancel_Implementation()
{
	if (bListenKey || IsDialogWindowActive())
	{
		return;
	}

	if (bCanSaveSettings)
	{
		CloseAndSaveBindigs();
	}
	else
	{
		bResetBindings = false;
		ShowDialogWindow();
	}
}

void USHKeyBindingsMenuWidget::ResetBind_Implementation()
{
	if (IsDialogWindowActive())
	{
		return;
	}

	if (bListenKey)
	{
		if (!Buttons.IsValidIndex(CurrentIndex))
		{
			return;
		}

		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
		if (!IsValid(BindButton))
		{
			return;
		}

		BindButton->ResetKey(BindButton->ActiveSlot);
		BindButton->SetKeyState(BindButton->ActiveSlot, ESHKeyButtonState::Calm);

		StopListenKey();
		CheckCanSaveSettings();
	}
	else
	{
		bResetBindings = true;
		ShowDialogWindow();
	}
}

void USHKeyBindingsMenuWidget::DeleteBind_Implementation()
{
	if (IsDialogWindowActive())
	{
		return;
	}

	if (!Buttons.IsValidIndex(CurrentIndex))
	{
		return;
	}

	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
	if (!IsValid(BindButton))
	{
		return;
	}

	BindButton->DeleteKey(BindButton->ActiveSlot);

	if (bListenKey)
	{
		BindButton->SetKeyState(BindButton->ActiveSlot, ESHKeyButtonState::Calm);
		StopListenKey();
	}

	CheckCanSaveSettings();
}

void USHKeyBindingsMenuWidget::MoveVertical_Implementation(int32 Offset)
{
	if (bListenKey)
	{
		return;
	}

	if (IsDialogWindowActive())
	{
		DialogWindow->MoveVertical(Offset);
		return;
	}

	if (!Buttons.IsValidIndex(CurrentIndex))
	{
		return;
	}

	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
	if (!IsValid(BindButton))
	{
		return;
	}

	EPlayerMappableKeySlot ActiveButtonSlot = BindButton->ActiveSlot;
	Super::MoveVertical_Implementation(Offset);

	if (CurrentIndex == 0)
	{
		ScrollBox->ScrollToStart();
	}
	else if (CurrentIndex == Buttons.Num() - 1)
	{
		ScrollBox->ScrollToEnd();
	}
	else
	{
		ScrollBox->ScrollWidgetIntoView(Buttons[CurrentIndex]);
	}

	if (Buttons.IsValidIndex(CurrentIndex))
	{
		ActiveButtonSlot == EPlayerMappableKeySlot::First ? ISHSlideButton::Execute_HorizontalMove(Buttons[CurrentIndex], -1) : ISHSlideButton::Execute_HorizontalMove(Buttons[CurrentIndex], 1);
	}
}

void USHKeyBindingsMenuWidget::MoveHorizontal_Implementation(int32 Offset)
{
	if (bListenKey)
	{
		return;
	}

	if (IsDialogWindowActive())
	{
		DialogWindow->MoveHorizontal(Offset);
		return;
	}

	Super::MoveHorizontal_Implementation(Offset);

	if (!Buttons.IsValidIndex(CurrentIndex))
	{
		return;
	}

	USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
	if (!IsValid(BindButton))
	{
		return;
	}
}

void USHKeyBindingsMenuWidget::OnResponse(bool bResponse)
{
	if (!bResponse)
	{
		HideDialogWindow();
		return;
	}

	if (bResetBindings)
	{
		ResetAllBindings();
		HideDialogWindow();

		return;
	}

	for(int32 i = 0; i < Buttons.Num(); i++)
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[i]);
		if (!IsValid(BindButton))
		{
			continue;
		}

		BindButton->UpdateData();
	}

	HideDialogWindow();
	
	bCanSaveSettings = true;
	CloseAndSaveBindigs();
}

void USHKeyBindingsMenuWidget::ShowDialogWindow()
{
	if (bResetBindings)
	{
		DialogWindow->SetDescription(ResetAllText);
	}
	else
	{
		DialogWindow->SetDescription(UnableSaveText);
	}

	DialogWindow->InFocus();
}

void USHKeyBindingsMenuWidget::HideDialogWindow()
{
	DialogWindow->OutFocus();

	SetDescription();
}

void USHKeyBindingsMenuWidget::StartListenKey()
{
	if (Buttons.IsValidIndex(CurrentIndex))
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
		if (IsValid(BindButton))
		{
			BindButton->SetKeyState(BindButton->ActiveSlot, ESHKeyButtonState::Active);
		}
	}

	FLatentActionInfo ActionInfo;
	ActionInfo.UUID = 2001;
	ActionInfo.Linkage = 0;
	ActionInfo.CallbackTarget = this;
	ActionInfo.ExecutionFunction = FName("SetListenKey");

	UKismetSystemLibrary::DelayUntilNextTick(GetWorld(), ActionInfo);

	SetBindingDescription();
}

void USHKeyBindingsMenuWidget::StopListenKey()
{
	bListenKey = false;

	SetDescription();
}

void USHKeyBindingsMenuWidget::SetListenKey()
{
	bListenKey = true;
}

bool USHKeyBindingsMenuWidget::IsDialogWindowActive() const
{
	if (IsValid(DialogWindow))
	{
		return DialogWindow->GetVisibility() == ESlateVisibility::Visible;
	}

	return false;
}

void USHKeyBindingsMenuWidget::CheckCanSaveSettings()
{
	bCanSaveSettings = true;

	for (int32 i = 0; i < Buttons.Num(); i++)
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[i]);
		if (!IsValid(BindButton))
		{
			continue;
		}

		BindButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Calm);
		BindButton->SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Calm);
	}

	for (int32 i = 0; i < Buttons.Num(); i++)
	{
		USHKeyBindButtonWidget* CurrentButton = Cast<USHKeyBindButtonWidget>(Buttons[i]);
		if (!IsValid(CurrentButton))
		{
			continue;
		}

		if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::First).IsValid())
		{
			if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::First) == CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::Second))
			{
				CurrentButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Warning);
				CurrentButton->SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Warning);

				bCanSaveSettings = false;
			}
		}
		else
		{
			CurrentButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Warning);
			bCanSaveSettings = false;
		}

		for (int32 j = 0; j < Buttons.Num(); j++)
		{
			USHKeyBindButtonWidget* LoopButton = Cast<USHKeyBindButtonWidget>(Buttons[j]);
			if (!IsValid(LoopButton) || i == j)
			{
				continue;
			}

			if (CurrentButton->GetMappingCategory() != LoopButton->GetMappingCategory())
			{
				continue;
			}

			if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::First).IsValid())
			{
				if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::First) == LoopButton->GetKeyForSlot(EPlayerMappableKeySlot::First))
				{
					LoopButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Warning);
					bCanSaveSettings = false;
				}

				if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::First) == LoopButton->GetKeyForSlot(EPlayerMappableKeySlot::Second))
				{
					LoopButton->SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Warning);
					bCanSaveSettings = false;
				}
			}

			if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::Second).IsValid())
			{
				if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::Second) == LoopButton->GetKeyForSlot(EPlayerMappableKeySlot::First))
				{
					LoopButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Warning);
					bCanSaveSettings = false;
				}

				if (CurrentButton->GetKeyForSlot(EPlayerMappableKeySlot::Second) == LoopButton->GetKeyForSlot(EPlayerMappableKeySlot::Second))
				{
					LoopButton->SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Warning);
					bCanSaveSettings = false;
				}
			}
		}
	}
}

void USHKeyBindingsMenuWidget::ResetAllBindings()
{
	for (int32 i = 0; i < Buttons.Num(); i++)
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[i]);
		if (!IsValid(BindButton))
		{
			continue;
		}

		BindButton->ResetKey(EPlayerMappableKeySlot::First);
		BindButton->ResetKey(EPlayerMappableKeySlot::Second);

		BindButton->SetKeyState(EPlayerMappableKeySlot::First, ESHKeyButtonState::Calm);
		BindButton->SetKeyState(EPlayerMappableKeySlot::Second, ESHKeyButtonState::Calm);
	}

	bCanSaveSettings = true;
}

void USHKeyBindingsMenuWidget::CloseAndSaveBindigs()
{
	if (!bCanSaveSettings)
	{
		return;
	}

	for (int32 i = 0; i < Buttons.Num(); i++)
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[i]);
		if (!IsValid(BindButton))
		{
			continue;
		}

		BindButton->SaveBindings();
	}

	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (IsValid(EISubsystem))
	{
		UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
		if (IsValid(UserSettings))
		{
			UserSettings->SaveSettings();
		}
	}

	OnCloseMenu.Broadcast();
}

void USHKeyBindingsMenuWidget::AnyKeyPressed(FKey PressedKey)
{
	if (!bListenKey || ValidatePressedKey(PressedKey))
	{
		return;
	}

	if (Buttons.IsValidIndex(CurrentIndex))
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Buttons[CurrentIndex]);
		if (IsValid(BindButton))
		{
			BindButton->SetKeyState(BindButton->ActiveSlot, ESHKeyButtonState::Calm);
			BindButton->BindKey(BindButton->ActiveSlot, PressedKey);
		}
	}

	StopListenKey();
	CheckCanSaveSettings();
}

bool USHKeyBindingsMenuWidget::ValidatePressedKey(FKey InKey) const
{
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (IsValid(EISubsystem))
	{
		UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
		if (IsValid(UserSettings))
		{
			UEnhancedPlayerMappableKeyProfile* CurrentKeyProfile = UserSettings->GetCurrentKeyProfile();
			if (IsValid(CurrentKeyProfile))
			{
				FMapPlayerKeyArgs Args = {};
				Args.MappingName = ResetBindMapping;
				Args.Slot = EPlayerMappableKeySlot::First;

				FPlayerKeyMapping* KeyMapping = CurrentKeyProfile->FindKeyMapping(Args);
				if (KeyMapping != nullptr && KeyMapping->GetCurrentKey() == InKey)
				{
					return true;
				}

				Args.MappingName = DeleteBindMapping;
				KeyMapping = CurrentKeyProfile->FindKeyMapping(Args);
				if (KeyMapping != nullptr && KeyMapping->GetCurrentKey() == InKey)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void USHKeyBindingsMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	check(DialogWindow);
	check(ScrollBox);

	DialogWindow->DialogWindowResponse.AddDynamic(this, &USHKeyBindingsMenuWidget::OnResponse);

	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (IsValid(EISubsystem))
	{
		UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
		if (IsValid(UserSettings))
		{
			if (!UserSettings->IsMappingContextRegistered(MovementMappingContext))
			{
				UserSettings->RegisterInputMappingContext(MovementMappingContext);
			}
			
			if (!UserSettings->IsMappingContextRegistered(MenuMappingContext))
			{
				UserSettings->RegisterInputMappingContext(MenuMappingContext);
			}
		}
	}

	for (USHBaseMenuButtonWidget* Button : Buttons)
	{
		USHKeyBindButtonWidget* BindButton = Cast<USHKeyBindButtonWidget>(Button);
		if (!IsValid(BindButton))
		{
			continue;
		}

		BindButton->UpdateData();
		BindButton->OnClicked.AddDynamic(this, &USHKeyBindingsMenuWidget::StartListenKey);
	}

	InitializeInputComponent();
	InputComponent->BindKey(EKeys::AnyKey, EInputEvent::IE_Released, this, &USHKeyBindingsMenuWidget::AnyKeyPressed).bExecuteWhenPaused = true;
}
