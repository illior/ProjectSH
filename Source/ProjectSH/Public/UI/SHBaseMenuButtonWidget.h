// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHBaseMenuButtonWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSHOnButtonClickSignature);

UCLASS()
class PROJECTSH_API USHBaseMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "ProjectSH: Events")
	FSHOnButtonClickSignature OnClicked;

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH", meta = (DisplayName = "StateChanged"))
	void ReceiveStateChanged();

	virtual void Click();

	virtual void SetState(ESHButtonState NewState);
	ESHButtonState GetState() const;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> ButtonImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Style")
	FSlateBrush NormalBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Style")
	FSlateBrush HoveredBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Style")
	FSlateBrush PressedBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Style")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Content")
	FText Text = FText();

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	ESHButtonState CurrentState = ESHButtonState::Normal;

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
