// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMappableKeySettings.h"
#include "SHPlayerMappableKeySettings.generated.h"

UENUM(BlueprintType)
enum class ESHMappableKeyCategory : uint8
{
	Movement,
	Menu
};

UCLASS()
class PROJECTSH_API USHPlayerMappableKeySettings : public UPlayerMappableKeySettings
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectSH: Settings")
	ESHMappableKeyCategory Category;
};
