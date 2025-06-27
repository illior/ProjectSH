// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "SHCoreTypes.h"
#include "SHKeyButtonWidget.generated.h"

UCLASS()
class PROJECTSH_API USHKeyButtonWidget : public USHBaseMenuButtonWidget
{
	GENERATED_BODY()


public:
	void SetKey(FKey InKey);
	FKey GetKey() const;

	void SetKeyState(ESHKeyButtonState InState);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> Background;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FSlateColor Calm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FSlateColor Active;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FSlateColor Warning;

	FKey CurrentKey;

	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
