// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuWidget.h"
#include "SHDialogWindowWidget.generated.h"

class UTextBlock;
class USHBaseMenuButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHOnDialogWindowResponseSignature, bool, bResponse);

UCLASS()
class PROJECTSH_API USHDialogWindowWidget : public USHBaseMenuWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "ProjectSH")
	FSHOnDialogWindowResponseSignature DialogWindowResponse;

	virtual void InFocus_Implementation() override;
	virtual void OutFocus_Implementation() override;

	void SetDescription(FText InText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> PositiveButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> NegativeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> ContentText;

	UFUNCTION()
	void PositiveButtonHandle();

	UFUNCTION()
	void NegativeButtonHandle();

	virtual void NativeOnInitialized() override;
};
