// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SHHUD.generated.h"

class USHKeysTextures;
class USHBaseInputWidget;

UCLASS()
class PROJECTSH_API ASHHUD : public AHUD
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	TSoftObjectPtr<UTexture2D> GetTextureForKey(FKey InKey) const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	TSoftObjectPtr<UTexture2D> GetTextureForMappingName(FName InMappingName) const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|Widgets")
	void SetInputWidget(USHBaseInputWidget* InWidget);

	USHBaseInputWidget* GetCurrentInputWidget() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<USHKeysTextures> KeysTextures;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|Widgets")
	TObjectPtr<USHBaseInputWidget> CurrentInputWidget;

	virtual void BeginPlay() override;
};
