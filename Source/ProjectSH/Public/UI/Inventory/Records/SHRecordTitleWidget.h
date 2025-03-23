// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "SHCoreTypes.h"
#include "SHRecordTitleWidget.generated.h"

UCLASS()
class PROJECTSH_API USHRecordTitleWidget : public USHBaseMenuButtonWidget
{
	GENERATED_BODY()


public:
	void SetRecord(FSHRecord InRecord);
	FSHRecord GetRecord() const { return Record; };

protected:
	FSHRecord Record;
};
