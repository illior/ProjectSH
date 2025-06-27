// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseInputWidget.h"
#include "SHPauseWidget.generated.h"

class UWidgetSwitcher;
class USHBaseMenuWidget;
class USHDialogWindowWidget;

UCLASS()
class PROJECTSH_API USHPauseWidget : public USHBaseInputWidget
{
	GENERATED_BODY()


public:

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuWidget> PauseMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHDialogWindowWidget> ReturnDialog;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MinFrequency = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	float FrequencyDecayRate = 1.0f;

	virtual void NativeOnInitialized() override;

	virtual void Move(FVector2D Value, float ElapsedTime) override;

	virtual void Apply() override;
	virtual void Cancel() override;

	virtual void ResetBind() override;
	virtual void DeleteBind() override;
};
