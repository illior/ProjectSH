// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHItemsCursorWidget.generated.h"

class UImage;
class UTextBlock;
class USHItemData;

UCLASS()
class PROJECTSH_API USHItemsCursorWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetSlotPadding(FMargin InPadding);
	FMargin GetSlotPadding() const;

	FIntPoint GetPosition() const;

	int32 GetSizeX() const;

	void SetActive(bool bActive);

	void SetPosition(FIntPoint InPosition, bool bBlend, ESHSlotState RightSlotState = ESHSlotState::Void);

	void SetTempItem(USHItemData* InItemData);
	USHItemData* GetTempItem() const;
	void ClearTempItem();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidget> CursorBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Inventory")
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Inventory")
	TObjectPtr<UTextBlock> ItemText;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FMargin SlotPadding = FMargin(0.0f, 0.0f, 10.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	float Speed = 550.0f;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TWeakObjectPtr<USHItemData> TempItem;

	bool bShouldMove = false;

	FIntPoint Position;
	FVector2D TargetPosition;

	UFUNCTION()
	void CountChanged(int32 NewCount);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
