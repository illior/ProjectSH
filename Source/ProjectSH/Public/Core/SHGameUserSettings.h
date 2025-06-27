// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SHGameUserSettings.generated.h"

UCLASS()
class PROJECTSH_API USHGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void ApplyCustomSettings();
};
