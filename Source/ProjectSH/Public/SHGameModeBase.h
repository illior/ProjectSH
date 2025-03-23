// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SHGameModeBase.generated.h"

class USHSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSHOnPauseStateChangedSignature, bool, bIsPaused);

UCLASS()
class PROJECTSH_API ASHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	ASHGameModeBase();

	UPROPERTY(BlueprintAssignable, Category = "ProjectSH")
	FSHOnPauseStateChangedSignature OnPauseStateChanged;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void LoadSaveGame(FString SlotName);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer);

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

protected:
	TObjectPtr<USHSaveGame> LoadGameInstance;
};
