// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHInputActionDescriptionWidget.h"
#include "SHInputAxisDescriptionWidget.generated.h"

class UPanelWidget;

UCLASS()
class PROJECTSH_API USHInputAxisDescriptionWidget : public USHInputActionDescriptionWidget
{
	GENERATED_BODY()


public:
	void SetKeyTexture(TSoftObjectPtr<UTexture2D> InTexture, int32 Index);
	void SetKeyDescription(FText InText);

	/**
	 * @param bAxis True = Vertical, False = Horizontal.
	 */
	void SetKeyDescription(FText InText, bool bAxis);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> KeyTopImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> KeyRightImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> KeyBottomImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UPanelWidget> VerticalPanel;

	virtual void NativeOnInitialized() override;
};
