// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHBaseMenuWidget.generated.h"

class USHBaseMenuButtonWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSHOnCloseMenuSignature);

UCLASS()
class PROJECTSH_API USHBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "ProjectSH")
	FSHOnCloseMenuSignature OnCloseMenu;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void SetButtonState(int32 Index, ESHButtonState InState);
	void SetButtonState(USHBaseMenuButtonWidget* InButton, ESHButtonState InState);

	virtual void SetButtonSelected(USHBaseMenuButtonWidget* InButton);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void InFocus();
	virtual void InFocus_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void OutFocus();
	virtual void OutFocus_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void Apply();
	virtual void Apply_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void Cancel();
	virtual void Cancel_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void ResetBind();
	virtual void ResetBind_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void DeleteBind();
	virtual void DeleteBind_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void MoveVertical(int32 Offset);
	virtual void MoveVertical_Implementation(int32 Offset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void MoveHorizontal(int32 Offset);
	virtual void MoveHorizontal_Implementation(int32 Offset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void AdditiveMoveVertical(int32 Offset);
	virtual void AdditiveMoveVertical_Implementation(int32 Offset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void AdditiveMoveHorizontal(int32 Offset);
	virtual void AdditiveMoveHorizontal_Implementation(int32 Offset);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "ProjectSH")
	TArray<USHBaseMenuButtonWidget*> Buttons;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectSH")
	int32 CurrentIndex = 0;

	virtual void NativeOnInitialized() override;
};
