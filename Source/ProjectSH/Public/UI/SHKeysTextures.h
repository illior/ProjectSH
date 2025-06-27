// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SHKeysTextures.generated.h"

UCLASS(BlueprintType)
class PROJECTSH_API USHKeysTextures : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH")
	TMap<FKey, TSoftObjectPtr<UTexture2D>> KeyToTextureMap;
};
