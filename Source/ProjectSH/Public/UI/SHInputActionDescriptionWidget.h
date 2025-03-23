// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHInputActionDescriptionWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTSH_API USHInputActionDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetKeyTexture(TSoftObjectPtr<UTexture2D> InTexture);
	void SetKeyDescription(FText InText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UTextBlock> TextDescription;

	virtual void NativeOnInitialized() override;
};
