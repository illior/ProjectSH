// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHBaseInputWidget.generated.h"

class UEnhancedInputComponent;
class UInputAction;
struct FInputActionInstance;

class USHInputActionDescriptionWidget;
class USHInputAxisDescriptionWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogBaseInputWidget, Log, All);

UCLASS()
class PROJECTSH_API USHBaseInputWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetCancelDescription(FText InText);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetApplyDescription(FText InText);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetResetBindDescription(FText InText);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetDeleteBindDescription(FText InText);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetMoveDescription(FText InText);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetVerticalAndHorizontalDescription(FText VerticalDesctiption, FText HorizontalDesctiption);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetAdditiveMoveDescription(FText InText);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|UI")
	void SetAdditiveVerticalAndHorizontalDescription(FText VerticalDesctiption, FText HorizontalDesctiption);

	virtual void Close();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputActionDescriptionWidget> ApplyDescription;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputActionDescriptionWidget> CancelDescription;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputActionDescriptionWidget> ResetBindDescription;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputActionDescriptionWidget> DeleteBindDescription;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputAxisDescriptionWidget> MoveVerticalDescription;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputAxisDescriptionWidget> MoveHorizontalDescription;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputAxisDescriptionWidget> AdditiveMoveVerticalDescription;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHInputAxisDescriptionWidget> AdditiveMoveHorizontalDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> ApplyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> AdditiveMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> ResetBindAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> DeleteBindAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float Frequency = 0.3f;

	FVector2D FrequencyMoveTime = FVector2D::ZeroVector;
	FVector2D FrequencyAdditiveMoveTime = FVector2D::ZeroVector;

	virtual void NativeOnInitialized() override;
	virtual void SetupInput(UEnhancedInputComponent* EnhancedInputComponent);

	virtual void Move(FVector2D Value, float ElapsedTime);
	virtual void AdditiveMove(FVector2D Value, float ElapsedTime);

	virtual void Apply();
	virtual void Cancel();

	virtual void ResetBind();
	virtual void DeleteBind();

	virtual void InputStartMove(const FInputActionInstance& Instance);
	virtual void InputMove(const FInputActionInstance& Instance);

	virtual void InputStartAdditiveMove(const FInputActionInstance& Instance);
	virtual void InputAdditiveMove(const FInputActionInstance& Instance);

	virtual void InputApply(const FInputActionInstance& Instance);
	virtual void InputCancel(const FInputActionInstance& Instance);

	virtual void InputResetBind(const FInputActionInstance& Instance);
	virtual void InputDeleteBind(const FInputActionInstance& Instance);
};
