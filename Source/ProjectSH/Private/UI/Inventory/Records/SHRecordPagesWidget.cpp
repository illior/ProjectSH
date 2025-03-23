// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Records/SHRecordPagesWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void USHRecordPagesWidget::SetRecord(FSHRecord InRecord)
{
	Record = InRecord;
	PageIndex = 0;

	Content->SetText(Record.Pages[PageIndex]);
	if (Record.Pages.Num() == 1)
	{
		Pages->SetText(FText());
	}
	else
	{
		Pages->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), PageIndex + 1, Record.Pages.Num())));
	}

	LeftButton->SetState(Record.Pages.IsValidIndex(PageIndex - 1) ? ESHButtonState::Normal : ESHButtonState::Disabled);
	RightButton->SetState(Record.Pages.IsValidIndex(PageIndex + 1) ? ESHButtonState::Normal : ESHButtonState::Disabled);
}

void USHRecordPagesWidget::MoveHorizontal(int32 Offset)
{
	int32 NextIndex = FMath::Clamp(PageIndex + Offset, 0, Record.Pages.Num() - 1);

	if (NextIndex != PageIndex)
	{
		PageIndex = NextIndex;

		Content->SetText(Record.Pages[PageIndex]);
		if (Record.Pages.Num() == 1)
		{
			Pages->SetText(FText());
		}
		else
		{
			Pages->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), PageIndex + 1, Record.Pages.Num())));
		}

		LeftButton->SetState(Record.Pages.IsValidIndex(PageIndex - 1) ? ESHButtonState::Normal : ESHButtonState::Disabled);
		RightButton->SetState(Record.Pages.IsValidIndex(PageIndex + 1) ? ESHButtonState::Normal : ESHButtonState::Disabled);
	}
}

void USHRecordPagesWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(Content);
	check(Pages);
	check(LeftButton);
	check(RightButton);
}
