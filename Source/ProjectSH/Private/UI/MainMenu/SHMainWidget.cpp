// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/SHMainWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Components/WidgetSwitcher.h"
#include "UI/SHBaseMenuWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "UI/SHDialogWindowWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void USHMainWidget::Continue()
{
	UGameplayStatics::OpenLevel(GetWorld(), NewGameLevelName, true, FString::Printf(TEXT("savegame=%s"), TEXT("DefaultSlot")));
}

void USHMainWidget::NewGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), NewGameLevelName, true);
}

void USHMainWidget::Exit()
{
	APlayerController* Controller = GetOwningPlayer();
	if (IsValid(Controller))
	{
		Controller->ConsoleCommand("quit");
	}
}

void USHMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(QuitGameDialog);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ContextOptions;
		ContextOptions.bNotifyUserSettings = true;

		Subsystem->AddMappingContext(MenuMappingContext, 0, ContextOptions);
	}
}

void USHMainWidget::Move(FVector2D Value, float ElapsedTime)
{
	USHBaseMenuWidget* MenuWidget;
	if (QuitGameDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		MenuWidget = QuitGameDialog;
	}
	else
	{
		MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	}

	if (!IsValid(MenuWidget))
	{
		return;
	}

	float DynamicFrequency = FMath::Max(MinFrequency, Frequency - ElapsedTime * FrequencyDecayRate);

	if (Value.Y != 0.0f && ElapsedTime > FrequencyMoveTime.Y)
	{
		FrequencyMoveTime.Y = ElapsedTime + DynamicFrequency;

		MenuWidget->MoveVertical(-Value.Y);
	}

	if (Value.X != 0.0f && ElapsedTime > FrequencyMoveTime.X)
	{
		FrequencyMoveTime.X = ElapsedTime + DynamicFrequency;

		MenuWidget->MoveHorizontal(Value.X);
	}
}

void USHMainWidget::AdditiveMove(FVector2D Value, float ElapsedTime)
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (!IsValid(MenuWidget))
	{
		return;
	}

	if (Value.X != 0.0f && ElapsedTime > FrequencyAdditiveMoveTime.X)
	{
		FrequencyAdditiveMoveTime.X = ElapsedTime + Frequency;

		MenuWidget->AdditiveMoveHorizontal(Value.X);
	}
}

void USHMainWidget::Apply()
{
	USHBaseMenuWidget* MenuWidget;
	if (QuitGameDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		MenuWidget = QuitGameDialog;
	}
	else
	{
		MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	}

	if (IsValid(MenuWidget))
	{
		MenuWidget->Apply();
	}
}

void USHMainWidget::Cancel()
{
	USHBaseMenuWidget* MenuWidget;
	if (QuitGameDialog->GetVisibility() == ESlateVisibility::Visible)
	{
		MenuWidget = QuitGameDialog;
	}
	else
	{
		MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	}

	if (IsValid(MenuWidget))
	{
		MenuWidget->Cancel();
	}
}

void USHMainWidget::ResetBind()
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (IsValid(MenuWidget))
	{
		MenuWidget->ResetBind();
	}
}

void USHMainWidget::DeleteBind()
{
	USHBaseMenuWidget* MenuWidget = Cast<USHBaseMenuWidget>(WidgetSwitcher->GetActiveWidget());
	if (IsValid(MenuWidget))
	{
		MenuWidget->DeleteBind();
	}
}
