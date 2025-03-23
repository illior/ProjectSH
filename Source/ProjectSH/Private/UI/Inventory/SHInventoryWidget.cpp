// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/SHInventoryWidget.h"
#include "InteractionSystem/SHInteractableTargetActor.h"
#include "EnhancedInputSubsystems.h"
#include "UI/SHHUD.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "UI/SHInputAxisDescriptionWidget.h"
#include "UI/Inventory/SHSubInventoryWidget.h"
#include "UI/Inventory/Map/SHMapWidget.h"
#include "UI/Inventory/Items/SHItemsWidget.h"
#include "UI/Inventory/Records/SHRecordsWidget.h"

void USHInventoryWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Visible)
	{
		APlayerController* PlayerController = GetOwningPlayer();
		if (IsValid(PlayerController))
		{
			ASHInteractableTargetActor* TargetActor = Cast<ASHInteractableTargetActor>(PlayerController->GetViewTarget());
			if (IsValid(TargetActor))
			{
				InteractTarget = TargetActor;

				Background->SetVisibility(ESlateVisibility::Hidden);
				Titles->SetVisibility(ESlateVisibility::Hidden);
				TabSwitcher->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				Background->SetVisibility(ESlateVisibility::HitTestInvisible);
				Titles->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				TabSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
	else if (InVisibility == ESlateVisibility::Collapsed)
	{
		TArray<UWidget*> TitlesIcon = TitlesPanel->GetAllChildren();
		if (TitlesIcon.IsValidIndex(TabSwitcher->GetActiveWidgetIndex()) && TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]->IsA<USHBaseMenuButtonWidget>())
		{
			USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]);
			Title->SetState(ESHButtonState::Normal);
		}

		InteractTarget.Reset();
	}
}

USHMapWidget* USHInventoryWidget::GetMapWidget() const
{
	for (UWidget* Widget : TabSwitcher->GetAllChildren())
	{
		if (Widget->IsA<USHMapWidget>())
		{
			return Cast<USHMapWidget>(Widget);
		}
	}

	return nullptr;
}

USHItemsWidget* USHInventoryWidget::GetItemsWidget() const
{
	for (UWidget* Widget : TabSwitcher->GetAllChildren())
	{
		if (Widget->IsA<USHItemsWidget>())
		{
			return Cast<USHItemsWidget>(Widget);
		}
	}

	return nullptr;
}

USHRecordsWidget* USHInventoryWidget::GetRecordsWidget() const
{
	for (UWidget* Widget : TabSwitcher->GetAllChildren())
	{
		if (Widget->IsA<USHRecordsWidget>())
		{
			return Cast<USHRecordsWidget>(Widget);
		}
	}

	return nullptr;
}

void USHInventoryWidget::OpenMap()
{
	SetVisibility(ESlateVisibility::Visible);

	USHMapWidget* MapWidget = GetMapWidget();
	if (IsValid(MapWidget))
	{
		TabSwitcher->SetActiveWidget(MapWidget);
		MapWidget->Show(false);
	}

	TArray<UWidget*> TitlesIcon = TitlesPanel->GetAllChildren();
	if (TitlesIcon.IsValidIndex(TabSwitcher->GetActiveWidgetIndex()) && TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]->IsA<USHBaseMenuButtonWidget>())
	{
		USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]);
		Title->SetState(ESHButtonState::Hovered);
	}
}

void USHInventoryWidget::OpenItems()
{
	SetVisibility(ESlateVisibility::Visible);

	USHItemsWidget* ItemsWidget = GetItemsWidget();
	if (IsValid(ItemsWidget))
	{
		TabSwitcher->SetActiveWidget(ItemsWidget);
		ItemsWidget->Show(false);
	}

	TArray<UWidget*> TitlesIcon = TitlesPanel->GetAllChildren();
	if (TitlesIcon.IsValidIndex(TabSwitcher->GetActiveWidgetIndex()) && TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]->IsA<USHBaseMenuButtonWidget>())
	{
		USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]);
		Title->SetState(ESHButtonState::Hovered);
	}
}

void USHInventoryWidget::OpenRecords()
{
	SetVisibility(ESlateVisibility::Visible);

	USHRecordsWidget* RecordsWidget = GetRecordsWidget();
	if (IsValid(RecordsWidget))
	{
		TabSwitcher->SetActiveWidget(RecordsWidget);
		RecordsWidget->Show(false);
	}

	TArray<UWidget*> TitlesIcon = TitlesPanel->GetAllChildren();
	if (TitlesIcon.IsValidIndex(TabSwitcher->GetActiveWidgetIndex()) && TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]->IsA<USHBaseMenuButtonWidget>())
	{
		USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]);
		Title->SetState(ESHButtonState::Hovered);
	}
}

void USHInventoryWidget::HideItems()
{
	TabSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void USHInventoryWidget::ShowItems()
{
	TabSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	USHItemsWidget* ItemsWidget = GetItemsWidget();
	if (IsValid(ItemsWidget))
	{
		ItemsWidget->Show(false);
	}
}

void USHInventoryWidget::SetCancelDescription(FText InText)
{
	CancelDescription->SetKeyDescription(InText);
}

void USHInventoryWidget::SetApplyDescription(FText InText)
{
	ApplyDescription->SetKeyDescription(InText);
}

void USHInventoryWidget::SetMoveDescription(FText InText)
{
	MoveVerticalDescription->SetKeyDescription(InText);
	MoveHorizontalDescription->SetKeyDescription(FText());
}

void USHInventoryWidget::SeterticalAndHorizontalDescription(FText VerticalDesctiption, FText HorizontalDesctiption)
{
	MoveVerticalDescription->SetKeyDescription(VerticalDesctiption, true);
	MoveHorizontalDescription->SetKeyDescription(HorizontalDesctiption, false);
}

void USHInventoryWidget::Close()
{
	if (InteractTarget.IsValid())
	{
		InteractTarget->HideItems();
		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Hide(false);
	}

	Super::Close();
}

void USHInventoryWidget::MappingsRebuilt()
{
	ASHHUD* HUD = IsValid(GetOwningPlayer()) ? GetOwningPlayer()->GetHUD<ASHHUD>() : nullptr;
	if (!IsValid(HUD))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	TArray<FKey> AdditiveMoveKeys = Subsystem->QueryKeysMappedToAction(AdditiveMoveAction);
	if (AdditiveMoveKeys.IsValidIndex(2))
	{
		LeftSwitch->SetBrushFromSoftTexture(HUD->GetTextureForKey(AdditiveMoveKeys[2]));
	}

	if (AdditiveMoveKeys.IsValidIndex(3))
	{
		RightSwitch->SetBrushFromSoftTexture(HUD->GetTextureForKey(AdditiveMoveKeys[3]));
	}

	TArray<FKey> ApplyKeys = Subsystem->QueryKeysMappedToAction(ApplyAction);
	if (ApplyKeys.IsValidIndex(0))
	{
		ApplyDescription->SetKeyTexture(HUD->GetTextureForKey(ApplyKeys[0]));
	}

	TArray<FKey> CancelKeys = Subsystem->QueryKeysMappedToAction(CancelAction);
	if (ApplyKeys.IsValidIndex(0))
	{
		CancelDescription->SetKeyTexture(HUD->GetTextureForKey(CancelKeys[0]));
	}

	TArray<FKey> MoveKeys = Subsystem->QueryKeysMappedToAction(MoveAction);
	for (int32 i = 0; i < MoveKeys.Num(); i++)
	{
		MoveVerticalDescription->SetKeyTexture(HUD->GetTextureForKey(MoveKeys[i]), i);
		MoveHorizontalDescription->SetKeyTexture(HUD->GetTextureForKey(MoveKeys[i]), i);
	}
}

void USHInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	Subsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &USHInventoryWidget::MappingsRebuilt);

	check(Background);
	check(Titles);
	check(TitlesPanel);
	check(TabSwitcher);

	check(LeftSwitch);
	check(RightSwitch);

	check(ApplyDescription);
	check(CancelDescription);
	check(MoveVerticalDescription);
	check(MoveHorizontalDescription);

	for (UWidget* Widget : TabSwitcher->GetAllChildren())
	{
		USHSubInventoryWidget* SubWidget = Cast<USHSubInventoryWidget>(Widget);
		if (SubWidget == nullptr)
		{
			continue;
		}

		SubWidget->Rename(nullptr, this);
		SubWidget->OnDisappearAnimationFinished.AddUObject(this, &USHInventoryWidget::SubWidgetDisappear);
	}
}

void USHInventoryWidget::SwitchTab(int32 Offset)
{
	SwitchIndex = TabSwitcher->GetActiveWidgetIndex() + Offset;
	if (SwitchIndex < 0 || SwitchIndex >= TabSwitcher->GetNumWidgets())
	{
		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Hide(true);
	}

	TArray<UWidget*> TitlesIcon = TitlesPanel->GetAllChildren();
	if (TitlesIcon.IsValidIndex(TabSwitcher->GetActiveWidgetIndex()) && TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]->IsA<USHBaseMenuButtonWidget>())
	{
		USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[TabSwitcher->GetActiveWidgetIndex()]);
		Title->SetState(ESHButtonState::Normal);
	}

	if (TitlesIcon.IsValidIndex(SwitchIndex) && TitlesIcon[SwitchIndex]->IsA<USHBaseMenuButtonWidget>())
	{
		USHBaseMenuButtonWidget* Title = Cast<USHBaseMenuButtonWidget>(TitlesIcon[SwitchIndex]);
		Title->SetState(ESHButtonState::Hovered);
	}
}

void USHInventoryWidget::SubWidgetDisappear()
{
	TabSwitcher->SetActiveWidgetIndex(SwitchIndex);

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Show(true);
	}
}

void USHInventoryWidget::Move(FVector2D Value, float ElapsedTime)
{
	if (InteractTarget.IsValid() && TabSwitcher->GetVisibility() == ESlateVisibility::Hidden)
	{
		InteractTarget->Move(Value, ElapsedTime);

		float DynamicFrequency = FMath::Max(MinFrequency, Frequency - ElapsedTime * FrequencyDecayRate);

		if (Value.X != 0 && ElapsedTime > FrequencyMoveTime.X)
		{
			FrequencyMoveTime.X = ElapsedTime + DynamicFrequency;

			InteractTarget->MoveWithFrequency(FVector2D(Value.X, 0.0f));
		}

		if (Value.Y != 0 && ElapsedTime > FrequencyMoveTime.Y)
		{
			FrequencyMoveTime.Y = ElapsedTime + DynamicFrequency;

			InteractTarget->MoveWithFrequency(FVector2D(0.0f, Value.Y));
		}

		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Move(Value, ElapsedTime);
		float DynamicFrequency = FMath::Max(MinFrequency, Frequency - ElapsedTime * FrequencyDecayRate);

		if (Value.X != 0 && ElapsedTime > FrequencyMoveTime.X)
		{
			FrequencyMoveTime.X = ElapsedTime + DynamicFrequency;

			ActiveWidget->MoveWithFrequency(FVector2D(Value.X, 0.0f));
		}

		if (Value.Y != 0 && ElapsedTime > FrequencyMoveTime.Y)
		{
			FrequencyMoveTime.Y = ElapsedTime + DynamicFrequency;

			ActiveWidget->MoveWithFrequency(FVector2D(0.0f, Value.Y));
		}
	}
}

void USHInventoryWidget::AdditiveMove(FVector2D Value, float ElapsedTime)
{
	if (InteractTarget.IsValid() && TabSwitcher->GetVisibility() == ESlateVisibility::Hidden)
	{
		InteractTarget->AdditiveMove(Value, ElapsedTime);

		if (Value.X != 0 && ElapsedTime > FrequencyAdditiveMoveTime.X)
		{
			FrequencyAdditiveMoveTime.X = ElapsedTime + Frequency;
			InteractTarget->AdditiveMoveWithFrequency(FVector2D(Value.X, 0.0f));
		}

		if (Value.Y != 0 && ElapsedTime > FrequencyAdditiveMoveTime.Y)
		{
			FrequencyAdditiveMoveTime.Y = ElapsedTime + Frequency;
			InteractTarget->AdditiveMoveWithFrequency(FVector2D(0.0f, Value.Y));

		}

		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (IsValid(ActiveWidget) && ActiveWidget->GetConsumeAdditiveMove())
	{
		ActiveWidget->AdditiveMove(Value, ElapsedTime);

		if (Value.X != 0 && ElapsedTime > FrequencyAdditiveMoveTime.X)
		{
			FrequencyAdditiveMoveTime.X = ElapsedTime + Frequency;
			ActiveWidget->AdditiveMoveWithFrequency(FVector2D(Value.X, 0.0f));
		}

		if (IsValid(ActiveWidget) && Value.Y != 0 && ElapsedTime > FrequencyAdditiveMoveTime.Y)
		{
			FrequencyAdditiveMoveTime.Y = ElapsedTime + Frequency;
			ActiveWidget->AdditiveMoveWithFrequency(FVector2D(0.0f, Value.Y));
		}

		return;
	}

	if (Value.X != 0 && ElapsedTime > FrequencyAdditiveMoveTime.X)
	{
		FrequencyAdditiveMoveTime.X = ElapsedTime + Frequency;
		SwitchTab(Value.X);
	}

	if (IsValid(ActiveWidget) && Value.Y != 0 && ElapsedTime > FrequencyAdditiveMoveTime.Y)
	{
		FrequencyAdditiveMoveTime.Y = ElapsedTime + Frequency;
		ActiveWidget->AdditiveMoveWithFrequency(FVector2D(0.0f, Value.Y));
	}
}

void USHInventoryWidget::Apply()
{
	if (InteractTarget.IsValid() && TabSwitcher->GetVisibility() == ESlateVisibility::Hidden)
	{
		InteractTarget->Apply();
		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Apply();
	}
}

void USHInventoryWidget::Cancel()
{
	if (InteractTarget.IsValid() && TabSwitcher->GetVisibility() == ESlateVisibility::Hidden)
	{
		InteractTarget->Cancel();
		return;
	}

	USHSubInventoryWidget* ActiveWidget = Cast<USHSubInventoryWidget>(TabSwitcher->GetActiveWidget());
	if (ActiveWidget != nullptr)
	{
		ActiveWidget->Cancel();
	}
}
