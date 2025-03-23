// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHRecordPagesWidget.generated.h"

class USHBaseMenuButtonWidget;
class URichTextBlock;
class UTextBlock;

UCLASS()
class PROJECTSH_API USHRecordPagesWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetRecord(FSHRecord InRecord);

	void MoveHorizontal(int32 Offset);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<URichTextBlock> Content;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> Pages;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> LeftButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> RightButton;

	FSHRecord Record;
	int32 PageIndex;

	virtual void NativeOnInitialized() override;
};
