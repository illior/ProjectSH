// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SHSubInventoryWidget.h"
#include "SHCoreTypes.h"
#include "SHRecordsWidget.generated.h"

class USHRecordTitleWidget;
class USHRecordPagesWidget;
class UScrollBox;

UCLASS()
class PROJECTSH_API USHRecordsWidget : public USHSubInventoryWidget
{
	GENERATED_BODY()


public:
	virtual void Show(bool bShowAnim) override;
	virtual void Hide(bool bShowAnim) override;

	virtual void Apply();
	virtual void Cancel();

	virtual void MoveWithFrequency(FVector2D Value);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHRecordPagesWidget> RecordPages;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	TSubclassOf<USHRecordTitleWidget> TitleClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	FMargin TitlePadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText ApplyDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText CancelDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText VerticalDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Description")
	FText HorizontalDescription;

	TArray<TObjectPtr<USHRecordTitleWidget>> Records;
	int32 CurrentIndex;

	void InventoryInitialized();
	void RecordAdded(FSHRecord NewRecord);

	virtual void NativeOnInitialized() override;
};
