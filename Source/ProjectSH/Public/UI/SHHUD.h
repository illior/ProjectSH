// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SHHUD.generated.h"

class UInputAction;
class USHKeysTextures;
class USHBaseInputWidget;
class USHInventoryWidget;

UENUM()
enum class FSHInventoryPages : uint8
{
	Map,
	Items,
	Records
};

UCLASS()
class PROJECTSH_API ASHHUD : public AHUD
{
	GENERATED_BODY()


public:
	void OpenInventoryWidget(FSHInventoryPages Page = FSHInventoryPages::Items);
	void OpenPauseWidget();

	void CloseWidgets();

	USHBaseInputWidget* GetPauseWidget() const { return PauseWidget; };
	USHInventoryWidget* GetInventoryWidget() const { return InventoryWidget; };

	TSoftObjectPtr<UTexture2D> GetTextureForKey(FKey InKey);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<USHKeysTextures> KeysTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Widgets")
	TSubclassOf<USHBaseInputWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Widgets")
	TSubclassOf<USHInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|Widgets")
	TObjectPtr<USHBaseInputWidget> PauseWidget;
	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|Widgets")
	TObjectPtr<USHInventoryWidget> InventoryWidget;

	virtual void BeginPlay() override;
};
