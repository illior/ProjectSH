// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHInteractWidget.generated.h"

class UImage;

UCLASS()
class PROJECTSH_API USHInteractWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UWidgetAnimation* GetShowAnim() const { return ShowAnim; }
	UWidgetAnimation* GetHideAnim() const { return HideAnim; }
	UWidgetAnimation* GetShowKeyAnim() const { return ShowKeyAnim; }
	UWidgetAnimation* GetHideKeyAnim() const { return HideKeyAnim; }

	void StartShow(bool WithKey);
	void StartHide();

	void StartShowKey();
	void StartHideKey();

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowKeyAnim;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideKeyAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSh")
	TObjectPtr<UImage> KeyCodeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH", meta = (GetOptions = "EnhancedInput.PlayerMappableKeySettings.GetKnownMappingNames"))
	FName MappingName;

	UFUNCTION()
	void MappingsRebuilt();

	virtual void NativeOnInitialized() override;
};
