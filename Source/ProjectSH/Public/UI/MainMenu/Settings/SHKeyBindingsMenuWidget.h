// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuWidget.h"
#include "SHKeyBindingsMenuWidget.generated.h"

class UScrollBox;
class USHDialogWindowWidget;

class UInputMappingContext;
enum class EPlayerMappableKeySlot : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogKeyBindings, Log, All);

UCLASS()
class PROJECTSH_API USHKeyBindingsMenuWidget : public USHBaseMenuWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDescription();

	UFUNCTION(BlueprintImplementableEvent)
	void SetBindingDescription();

	virtual void SetButtonSelected(USHBaseMenuButtonWidget* InButton) override;

	void InFocus_Implementation() override;
	void OutFocus_Implementation() override;

	virtual void Apply_Implementation() override;
	virtual void Cancel_Implementation() override;
	virtual void ResetBind_Implementation() override;
	virtual void DeleteBind_Implementation() override;
	virtual void MoveVertical_Implementation(int32 Offset) override;
	virtual void MoveHorizontal_Implementation(int32 Offset) override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHDialogWindowWidget> DialogWindow;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectSH", meta = (MultiLine = "true"))
	FText ResetAllText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectSH", meta = (MultiLine = "true"))
	FText UnableSaveText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectSH", meta = (GetOptions = "EnhancedInput.PlayerMappableKeySettings.GetKnownMappingNames"))
	FName ResetBindMapping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectSH", meta = (GetOptions = "EnhancedInput.PlayerMappableKeySettings.GetKnownMappingNames"))
	FName DeleteBindMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> MovementMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> MenuMappingContext;

	bool bListenKey = false;
	bool bResetBindings = false;
	bool bCanSaveSettings = true;

	UFUNCTION()
	void OnResponse(bool bResponse);

	void ShowDialogWindow();
	void HideDialogWindow();

	UFUNCTION()
	void StartListenKey();
	void StopListenKey();

	UFUNCTION()
	void SetListenKey();

	bool IsDialogWindowActive() const;

	void CheckCanSaveSettings();
	void ResetAllBindings();
	void CloseAndSaveBindigs();

	void AnyKeyPressed(FKey PressedKey);
	bool ValidatePressedKey(FKey InKey) const;

	virtual void NativeOnInitialized() override;
};
