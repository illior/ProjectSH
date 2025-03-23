// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Records/SHRecordTitleWidget.h"
#include "Components/TextBlock.h"

void USHRecordTitleWidget::SetRecord(FSHRecord InRecord)
{
	Record = InRecord;

	Text = Record.Title;
	ButtonText->SetText(Text);
}
