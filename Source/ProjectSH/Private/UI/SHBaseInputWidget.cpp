// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SHBaseInputWidget.h"
#include "Player/SHPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "UI/SHInputAxisDescriptionWidget.h"

DEFINE_LOG_CATEGORY(LogBaseInputWidget);

void USHBaseInputWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Visible)
	{
		RegisterInputComponent();
	}
	else if (InVisibility == ESlateVisibility::Collapsed)
	{
		UnregisterInputComponent();
	}
}

void USHBaseInputWidget::SetCancelDescription(FText InText)
{
	CancelDescription->SetKeyDescription(InText);
}

void USHBaseInputWidget::SetApplyDescription(FText InText)
{
	ApplyDescription->SetKeyDescription(InText);
}

void USHBaseInputWidget::SetResetBindDescription(FText InText)
{
	ResetBindDescription->SetKeyDescription(InText);
}

void USHBaseInputWidget::SetDeleteBindDescription(FText InText)
{
	DeleteBindDescription->SetKeyDescription(InText);
}

void USHBaseInputWidget::SetMoveDescription(FText InText)
{
	MoveVerticalDescription->SetKeyDescription(InText);
	MoveHorizontalDescription->SetKeyDescription(FText());
}

void USHBaseInputWidget::SetVerticalAndHorizontalDescription(FText VerticalDesctiption, FText HorizontalDesctiption)
{
	MoveVerticalDescription->SetKeyDescription(VerticalDesctiption, true);
	MoveHorizontalDescription->SetKeyDescription(HorizontalDesctiption, false);
}

void USHBaseInputWidget::SetAdditiveMoveDescription(FText InText)
{
	AdditiveMoveVerticalDescription->SetKeyDescription(InText);
	AdditiveMoveHorizontalDescription->SetKeyDescription(FText());
}

void USHBaseInputWidget::SetAdditiveVerticalAndHorizontalDescription(FText VerticalDesctiption, FText HorizontalDesctiption)
{
	AdditiveMoveVerticalDescription->SetKeyDescription(VerticalDesctiption, true);
	AdditiveMoveHorizontalDescription->SetKeyDescription(HorizontalDesctiption, false);
}

void USHBaseInputWidget::Close()
{
	ASHPlayerController* PlayerController = GetOwningPlayer<ASHPlayerController>();
	if (PlayerController != nullptr)
	{
		PlayerController->ResumeGame();
	}
}

void USHBaseInputWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializeInputComponent();
	SetupInput(CastChecked<UEnhancedInputComponent>(InputComponent));
}

void USHBaseInputWidget::SetupInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &USHBaseInputWidget::InputStartMove);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &USHBaseInputWidget::InputMove);

	EnhancedInputComponent->BindAction(AdditiveMoveAction, ETriggerEvent::Started, this, &USHBaseInputWidget::InputStartAdditiveMove);
	EnhancedInputComponent->BindAction(AdditiveMoveAction, ETriggerEvent::Triggered, this, &USHBaseInputWidget::InputAdditiveMove);

	EnhancedInputComponent->BindAction(ApplyAction, ETriggerEvent::Completed, this, &USHBaseInputWidget::InputApply);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Completed, this, &USHBaseInputWidget::InputCancel);

	EnhancedInputComponent->BindAction(ResetBindAction, ETriggerEvent::Completed, this, &USHBaseInputWidget::InputResetBind);
	EnhancedInputComponent->BindAction(DeleteBindAction, ETriggerEvent::Completed, this, &USHBaseInputWidget::InputDeleteBind);
}

void USHBaseInputWidget::Move(FVector2D Value, float ElapsedTime)
{
	UE_LOG(LogBaseInputWidget, Display, TEXT("Move. Value: %s, ElapsedTime: %f"), *Value.ToString(), ElapsedTime);
}

void USHBaseInputWidget::AdditiveMove(FVector2D Value, float ElapsedTime)
{
	UE_LOG(LogBaseInputWidget, Display, TEXT("AdditiveMove. Value: %s, ElapsedTime: %f"), *Value.ToString(), ElapsedTime);
}

void USHBaseInputWidget::Apply()
{
	UE_LOG(LogBaseInputWidget, Display, TEXT("Apply"));
}

void USHBaseInputWidget::Cancel()
{
	UE_LOG(LogBaseInputWidget, Display, TEXT("Cancel"));

	Close();
}

void USHBaseInputWidget::ResetBind()
{

}

void USHBaseInputWidget::DeleteBind()
{

}

void USHBaseInputWidget::InputStartMove(const FInputActionInstance& Instance)
{
	FrequencyMoveTime = FVector2D::ZeroVector;
}

void USHBaseInputWidget::InputMove(const FInputActionInstance& Instance)
{
	Move(Instance.GetValue().Get<FVector2D>(), Instance.GetElapsedTime());
}

void USHBaseInputWidget::InputStartAdditiveMove(const FInputActionInstance& Instance)
{
	FrequencyAdditiveMoveTime = FVector2D::ZeroVector;
}

void USHBaseInputWidget::InputAdditiveMove(const FInputActionInstance& Instance)
{
	AdditiveMove(Instance.GetValue().Get<FVector2D>(), Instance.GetElapsedTime());
}

void USHBaseInputWidget::InputApply(const FInputActionInstance& Instance)
{
	Apply();
}

void USHBaseInputWidget::InputCancel(const FInputActionInstance& Instance)
{
	Cancel();
}

void USHBaseInputWidget::InputResetBind(const FInputActionInstance& Instance)
{
	ResetBind();
}

void USHBaseInputWidget::InputDeleteBind(const FInputActionInstance& Instance)
{
	DeleteBind();
}
