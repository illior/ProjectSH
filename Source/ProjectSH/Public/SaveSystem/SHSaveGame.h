// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SHCoreTypes.h"
#include "SHSaveGame.generated.h"

UCLASS()
class PROJECTSH_API USHSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY()
	TMap<FName, FSHActorSaveData> SavedActors;

	UPROPERTY()
	FSHPlayerSaveData PlayerData;
};
