// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Records/SHRecordsWidget.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/Inventory/Records/SHRecordTitleWidget.h"
#include "UI/Inventory/Records/SHRecordPagesWidget.h"
#include "Components/SHInventoryComponent.h"

void USHRecordsWidget::Show(bool bShowAnim)
{
	Super::Show(bShowAnim);

	CurrentIndex = 0;
	if (Records.IsValidIndex(CurrentIndex))
	{
		Records[CurrentIndex]->SetState(ESHButtonState::Hovered);
		RecordPages->SetRecord(Records[CurrentIndex]->GetRecord());

		ScrollBox->ScrollWidgetIntoView(Records[CurrentIndex]);
	}

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetApplyDescription(ApplyDescription);
		InventoryWidget->SetCancelDescription(CancelDescription);

		InventoryWidget->SeterticalAndHorizontalDescription(VerticalDescription, HorizontalDescription);
	}
}

void USHRecordsWidget::Hide(bool bShowAnim)
{
	Super::Hide(bShowAnim);

	if (Records.IsValidIndex(CurrentIndex))
	{
		Records[CurrentIndex]->SetState(ESHButtonState::Normal);
	}
}

void USHRecordsWidget::Apply()
{
	UE_LOG(LogTemp, Display, TEXT("Records Apply"));
	RecordPages->MoveHorizontal(1);
}

void USHRecordsWidget::Cancel()
{
	UE_LOG(LogTemp, Display, TEXT("Records Cancel"));
	Close();
}

void USHRecordsWidget::MoveWithFrequency(FVector2D Value)
{
	if (Value.Y != 0)
	{
		int32 NextIndex = FMath::Clamp(CurrentIndex - Value.Y, 0, Records.Num() - 1);

		if (NextIndex != CurrentIndex)
		{
			Records[CurrentIndex]->SetState(ESHButtonState::Normal);
			CurrentIndex = NextIndex;

			Records[CurrentIndex]->SetState(ESHButtonState::Hovered);
			RecordPages->SetRecord(Records[CurrentIndex]->GetRecord());

			ScrollBox->ScrollWidgetIntoView(Records[CurrentIndex]);
		}
	}

	if (Value.X != 0)
	{
		RecordPages->MoveHorizontal(Value.X);
	}
}

void USHRecordsWidget::InventoryInitialized()
{
	USHInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (!IsValid(CharacterInventory))
	{
		return;
	}

	ScrollBox->ClearChildren();
	
	for (TIndexedContainerIterator It = CharacterInventory->GetRecordsIterator(); It; ++It)
	{
		USHRecordTitleWidget* TitleWidget = CreateWidget<USHRecordTitleWidget>(GetOwningPlayer(), TitleClass);
		if (!IsValid(TitleWidget))
		{
			continue;
		}

		TitleWidget->SetRecord(*It);

		UScrollBoxSlot* ScrollSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(TitleWidget));
		if (IsValid(ScrollSlot))
		{
			ScrollSlot->SetPadding(TitlePadding);
		}

		Records.Add(TitleWidget);
	}
}

void USHRecordsWidget::RecordAdded(FSHRecord NewRecord)
{
	USHRecordTitleWidget* TitleWidget = CreateWidget<USHRecordTitleWidget>(GetOwningPlayer(), TitleClass);
	if (IsValid(TitleWidget))
	{
		TitleWidget->SetRecord(NewRecord);

		UScrollBoxSlot* ScrollSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(TitleWidget));
		if (IsValid(ScrollSlot))
		{
			ScrollSlot->SetPadding(TitlePadding);
		}

		Records.Add(TitleWidget);

		if (Records.IsValidIndex(CurrentIndex))
		{
			Records[CurrentIndex]->SetState(ESHButtonState::Normal);
		}

		CurrentIndex = Records.Num() - 1;

		Records[CurrentIndex]->SetState(ESHButtonState::Hovered);
		RecordPages->SetRecord(Records[CurrentIndex]->GetRecord());

		ScrollBox->ScrollWidgetIntoView(Records[CurrentIndex]);
	}
}

void USHRecordsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(RecordPages);
	check(ScrollBox);

	USHInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (!IsValid(CharacterInventory))
	{
		return;
	}

	CharacterInventory->OnInventoryInitialized.AddUObject(this, &USHRecordsWidget::InventoryInitialized);
	CharacterInventory->OnRecordAdded.AddUObject(this, &USHRecordsWidget::RecordAdded);
}
