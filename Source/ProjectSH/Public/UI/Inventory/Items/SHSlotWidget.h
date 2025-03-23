// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHSlotWidget.generated.h"

class UImage;

UCLASS()
class PROJECTSH_API USHSlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetSlotState(ESHSlotState InState);
	ESHSlotState GetSlotState() const { return SlotState; }

	void SetSelected();
	void SetUnselected();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> SelectTexture;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Appearance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	FLinearColor SelectedTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.7f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	FLinearColor UnselectedTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	ESHSlotState SlotState = ESHSlotState::Void;

	virtual void NativeOnInitialized() override;
};
