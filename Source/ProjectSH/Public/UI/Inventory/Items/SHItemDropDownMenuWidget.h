// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuWidget.h"
#include "SHItemDropDownMenuWidget.generated.h"

class USHItemData;
class FSHOnButtonClickSignature;

UCLASS()
class PROJECTSH_API USHItemDropDownMenuWidget : public USHBaseMenuWidget
{
	GENERATED_BODY()


public:
	void Open(USHItemData* InItemData);
	void OpenToUse(USHItemData* InItemData);
	void Close();

	FSHOnButtonClickSignature& GetInspectButtonSignature();
	FSHOnButtonClickSignature& GetEquipButtonSignature();
	FSHOnButtonClickSignature& GetFastAccessButtonSignature();
	FSHOnButtonClickSignature& GetCombineButtonSignature();
	FSHOnButtonClickSignature& GetUseButtonSignature();
	FSHOnButtonClickSignature& GetDropButtonSignature();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> InspectButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> EquipButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> FastAccessButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> CombineButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> UseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> DropButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	float ButtonHeight = 27.0f;

	void CalculateHeight(int32 ButtonsCount);

	virtual void NativeOnInitialized() override;
};
