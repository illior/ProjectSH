// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SHCoreTypes.h"
#include "SHPlayerState.generated.h"

UCLASS()
class PROJECTSH_API ASHPlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	FSHPlayerSaveData GetPlayerData();

	void LoadState(FSHPlayerSaveData InSaveData);
	void LoadCharacter(FSHPlayerSaveData InSaveData);
};
