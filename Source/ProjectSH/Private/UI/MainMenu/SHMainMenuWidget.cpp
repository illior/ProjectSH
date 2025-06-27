// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/SHMainMenuWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"

FSHOnButtonClickSignature& USHMainMenuWidget::GetContinueButtonSignature()
{
	return ContinueButton->OnClicked;
}

FSHOnButtonClickSignature& USHMainMenuWidget::GetNewGameButtonSignature()
{
	return NewGameButton->OnClicked;
}

FSHOnButtonClickSignature& USHMainMenuWidget::GetLoadGameButtonSignature()
{
	return LoadGameButton->OnClicked;
}

FSHOnButtonClickSignature& USHMainMenuWidget::GetSettingsButtonSignature()
{
	return SettingsButton->OnClicked;
}

FSHOnButtonClickSignature& USHMainMenuWidget::GetExitButtonSignature()
{
	return ExitButton->OnClicked;
}

void USHMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(ContinueButton);
	Buttons.Add(ContinueButton);

	check(NewGameButton);
	Buttons.Add(NewGameButton);

	check(LoadGameButton);
	Buttons.Add(LoadGameButton);

	check(SettingsButton);
	Buttons.Add(SettingsButton);

	check(ExitButton);
	Buttons.Add(ExitButton);

	SetButtonState(ContinueButton, ESHButtonState::Hovered);
}
