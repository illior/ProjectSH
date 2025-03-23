// Fill out your copyright notice in the Description page of Project Settings.

#include "SHGameModeBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SHPlayerState.h"
#include "SaveSystem/SHSavableActor.h"
#include "SaveSystem/SHSaveGame.h"

ASHGameModeBase::ASHGameModeBase()
{

}

void ASHGameModeBase::WriteSaveGame()
{
	USHSaveGame* SaveInstance = Cast<USHSaveGame>(UGameplayStatics::CreateSaveGameObject(USHSaveGame::StaticClass()));
	SaveInstance->SavedActors.Empty();

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!IsValid(Actor) || !Actor->Implements<USHSavableActor>())
		{
			continue;
		}

		SaveInstance->SavedActors.Add(Actor->GetFName(), ISHSavableActor::Execute_GetSaveData(Actor));
	}

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASHPlayerState::StaticClass(), Players);
	if (Players.Num() > 0)
	{
		if (ASHPlayerState* PlayerState = Cast<ASHPlayerState>(Players[0]))
		{
			SaveInstance->PlayerData = PlayerState->GetPlayerData();
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveInstance, FString(TEXT("DefaultSlot")), 0);
}

void ASHGameModeBase::LoadSaveGame(FString SlotName)
{
	LoadGameInstance = Cast<USHSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (LoadGameInstance == nullptr)
	{
		return;
	}

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!IsValid(Actor) || !Actor->Implements<USHSavableActor>())
		{
			continue;
		}

		if (FSHActorSaveData* ActorData = LoadGameInstance->SavedActors.Find(Actor->GetFName()))
		{
			ISHSavableActor::Execute_LoadFromSaveData(Actor, *ActorData);
		}
		else
		{
			Actor->Destroy();
		}
	}
}

void ASHGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0)
	{
		LoadSaveGame(SelectedSaveSlot);
	}
}

void ASHGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ASHPlayerState* PlayerState = NewPlayer->GetPlayerState<ASHPlayerState>();
	if (IsValid(PlayerState) && IsValid(LoadGameInstance))
	{
		PlayerState->LoadState(LoadGameInstance->PlayerData);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (IsValid(PlayerState) && IsValid(LoadGameInstance))
	{
		PlayerState->LoadCharacter(LoadGameInstance->PlayerData);
	}
}

bool ASHGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	OnPauseStateChanged.Broadcast(true);

	return Super::SetPause(PC, CanUnpauseDelegate);
}

bool ASHGameModeBase::ClearPause()
{
	OnPauseStateChanged.Broadcast(false);

	return Super::ClearPause();
}
