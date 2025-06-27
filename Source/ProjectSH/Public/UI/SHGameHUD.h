// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHHUD.h"
#include "SHGameHUD.generated.h"

class USHPauseWidget;
class USHInventoryWidget;

UENUM()
enum class FSHInventoryPages : uint8
{
	Map,
	Items,
	Records
};

UCLASS()
class PROJECTSH_API ASHGameHUD : public ASHHUD
{
	GENERATED_BODY()


public:
	void OpenInventoryWidget(FSHInventoryPages Page = FSHInventoryPages::Items);
	void OpenPauseWidget();

	void CloseWidgets();

	USHBaseInputWidget* GetPauseWidget() const;
	USHInventoryWidget* GetInventoryWidget() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Widgets")
	TSubclassOf<USHPauseWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Widgets")
	TSubclassOf<USHInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|Widgets")
	TObjectPtr<USHPauseWidget> PauseWidget;
	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|Widgets")
	TObjectPtr<USHInventoryWidget> InventoryWidget;

	virtual void BeginPlay() override;
};
