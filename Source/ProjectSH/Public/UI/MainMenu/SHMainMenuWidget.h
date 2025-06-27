// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHBaseMenuWidget.h"
#include "SHMainMenuWidget.generated.h"

class FSHOnButtonClickSignature;

UCLASS()
class PROJECTSH_API USHMainMenuWidget : public USHBaseMenuWidget
{
	GENERATED_BODY()


public:
	FSHOnButtonClickSignature& GetContinueButtonSignature();
	FSHOnButtonClickSignature& GetNewGameButtonSignature();
	FSHOnButtonClickSignature& GetLoadGameButtonSignature();
	FSHOnButtonClickSignature& GetSettingsButtonSignature();
	FSHOnButtonClickSignature& GetExitButtonSignature();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> ContinueButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> LoadGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> SettingsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<USHBaseMenuButtonWidget> ExitButton;

	virtual void NativeOnInitialized() override;
};
