// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseInputWidget.h"
#include "SHMainWidget.generated.h"

class UWidgetSwitcher;
class USHBaseMenuWidget;
class USHDialogWindowWidget;
class UInputMappingContext;

UCLASS()
class PROJECTSH_API USHMainWidget : public USHBaseInputWidget
{
	GENERATED_BODY()


public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> MenuMappingContext;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuWidget> MainMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuWidget> LoadGameMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuWidget> SettingsMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHDialogWindowWidget> QuitGameDialog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	FName NewGameLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MinFrequency = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	float FrequencyDecayRate = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void Continue();
	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void NewGame();
	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void Exit();

	virtual void NativeOnInitialized() override;

	virtual void Move(FVector2D Value, float ElapsedTime) override;
	virtual void AdditiveMove(FVector2D Value, float ElapsedTime) override;

	virtual void Apply() override;
	virtual void Cancel() override;

	virtual void ResetBind() override;
	virtual void DeleteBind() override;
};
