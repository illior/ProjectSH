// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SHPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PROJECTSH_API ASHPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ASHPlayerController();

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void ResumeGame();

	void OpenPauseMenu();

	void OpenItems();
	void OpenMap();
	void OpenRecords();

	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> CharacterMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> PauseMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputMappingContext> OpenUIMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> OpenPauseMenuAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> OpenMapAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> OpenItemsAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> OpenRecordsAction;

	void PauseGame();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	void InputPause(const FInputActionValue& Value);
	void InputMap(const FInputActionValue& Value);
	void InputItems(const FInputActionValue& Value);
	void InputRecords(const FInputActionValue& Value);
};
