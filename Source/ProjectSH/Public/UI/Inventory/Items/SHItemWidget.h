// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHItemWidget.generated.h"

class UTextBlock;
class UImage;

class USHItemData;

UCLASS()
class PROJECTSH_API USHItemWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetItemData(USHItemData* InItemData);
	USHItemData* GetItemData() const;

	void SetSelected();
	void SetUnselected();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> ItemTexture;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> CountText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> EquipText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> FastAccessText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> SelectTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	FLinearColor SelectedTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.7f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	FLinearColor UnselectedTint = FLinearColor(1.0f, 1.0f, 1.0f, 0.0f);

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TWeakObjectPtr<USHItemData> ItemData;

	UFUNCTION()
	void PositionChanged(FIntPoint OldPosition, FIntPoint NewPosition);
	UFUNCTION()
	void CountChanged(int32 NewCount);

	void UpdateData();
	void UpdatePosition();

	virtual void NativeOnInitialized() override;
};
