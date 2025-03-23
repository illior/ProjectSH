// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHBaseInputWidget.generated.h"

class UEnhancedInputComponent;
class UInputAction;
struct FInputActionInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogBaseInputWidget, Log, All);

UCLASS()
class PROJECTSH_API USHBaseInputWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	virtual void Close();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> ApplyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> AdditiveMoveAction;

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

	virtual void InputStartMove(const FInputActionInstance& Instance);
	virtual void InputMove(const FInputActionInstance& Instance);

	virtual void InputStartAdditiveMove(const FInputActionInstance& Instance);
	virtual void InputAdditiveMove(const FInputActionInstance& Instance);

	virtual void InputApply(const FInputActionInstance& Instance);
	virtual void InputCancel(const FInputActionInstance& Instance);
};
