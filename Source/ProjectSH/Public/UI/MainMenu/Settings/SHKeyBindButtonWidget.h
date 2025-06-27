// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuButtonWidget.h"
#include "UI/SHSlideButton.h"
#include "SHKeyBindButtonWidget.generated.h"

enum class EPlayerMappableKeySlot : uint8;
class UEnhancedPlayerMappableKeyProfile;

enum class ESHMappableKeyCategory : uint8;
class USHKeyButtonWidget;
class UImage;

UCLASS()
class PROJECTSH_API USHKeyBindButtonWidget : public USHBaseMenuButtonWidget, public ISHSlideButton
{
	GENERATED_BODY()


public:
	EPlayerMappableKeySlot ActiveSlot;

	void UpdateData();
	
	void SetActiveKey(USHKeyButtonWidget* InKeyButton);

	void SetKeyState(EPlayerMappableKeySlot KeySlot, ESHKeyButtonState KeyButtonState);
	void BindKey(EPlayerMappableKeySlot KeySlot, FKey InKey);
	void ResetKey(EPlayerMappableKeySlot KeySlot);
	void DeleteKey(EPlayerMappableKeySlot KeySlot);

	void SaveBindings();

	FKey GetKeyForSlot(EPlayerMappableKeySlot KeySlot) const;
	ESHMappableKeyCategory GetMappingCategory() const;

	virtual void SetState(ESHButtonState NewState) override;
	virtual void HorizontalMove_Implementation(int32 Offset) override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHKeyButtonWidget> FirstKeyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHKeyButtonWidget> SecondKeyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH", meta = (GetOptions = "EnhancedInput.PlayerMappableKeySettings.GetKnownMappingNames"))
	FName MappingName;

	ESHMappableKeyCategory MappingCategory;

	virtual void NativeOnInitialized() override;

	UEnhancedPlayerMappableKeyProfile* GetCurrentKeyProfile() const;
	ESHMappableKeyCategory GetCurrentMappingCategory() const;
};
