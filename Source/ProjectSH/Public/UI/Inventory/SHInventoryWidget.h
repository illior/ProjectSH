// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseInputWidget.h"
#include "SHInventoryWidget.generated.h"

class ASHInteractableTargetActor;
class USHMapWidget;
class USHItemsWidget;
class USHRecordsWidget;
class UPanelWidget;
class UWidgetSwitcher;
class UImage;

UCLASS()
class PROJECTSH_API USHInventoryWidget : public USHBaseInputWidget
{
	GENERATED_BODY()


public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	USHMapWidget* GetMapWidget() const;
	USHItemsWidget* GetItemsWidget() const;
	USHRecordsWidget* GetRecordsWidget() const;

	void SetTitleVisibility(ESlateVisibility InVisibility);

	void OpenMap();
	void OpenItems();
	void OpenRecords();

	void HideItems();
	void ShowItems();

	virtual void Close() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidget> Background;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidget> Titles;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidgetSwitcher> TabSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UPanelWidget> TitlesPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MinFrequency = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	float FrequencyDecayRate = 1.0f;

	TWeakObjectPtr<ASHInteractableTargetActor> InteractTarget;

	int32 SwitchIndex;

	virtual void NativeOnInitialized() override;

	void SwitchTab(int32 Offset);

	void SubWidgetDisappear();

	virtual void Move(FVector2D Value, float ElapsedTime);
	virtual void AdditiveMove(FVector2D Value, float ElapsedTime);

	virtual void Apply();
	virtual void Cancel();
};
