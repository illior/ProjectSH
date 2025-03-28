// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SHPlayerState.h"
#include "SaveSystem/SHSavableComponent.h"

FSHPlayerSaveData ASHPlayerState::GetPlayerData()
{
	FSHPlayerSaveData SaveData = FSHPlayerSaveData();

	APawn* Pawn = GetPawn();
	if (Pawn != nullptr)
	{
		SaveData.Position = Pawn->GetActorLocation();
		SaveData.Rotation = Pawn->GetActorRotation();
		SaveData.ControlRotation = Pawn->GetControlRotation();

		FMemoryWriter Writer = FMemoryWriter(SaveData.CharacterByteData);
		FObjectAndNameAsStringProxyArchive Ar(Writer, false);
		Ar.ArIsSaveGame = true;
		Ar.ArNoDelta = true;

		Pawn->Serialize(Ar);

		SaveData.ComponentsData.Empty();
		TSet<UActorComponent*> Components = Pawn->GetComponents();
		for (UActorComponent* Component : Components)
		{
			if (!IsValid(Component) || !Component->Implements<USHSavableComponent>())
			{
				continue;
			}

			SaveData.ComponentsData.Add(Component->GetFName(), ISHSavableComponent::Execute_GetSaveData(Component));
		}
	}

	FMemoryWriter Writer = FMemoryWriter(SaveData.PlayerStateByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);

	return SaveData;
}

void ASHPlayerState::LoadState(FSHPlayerSaveData InSaveData)
{
	FMemoryReader Reader = FMemoryReader(InSaveData.PlayerStateByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);
}

void ASHPlayerState::LoadCharacter(FSHPlayerSaveData InSaveData)
{
	APawn* Pawn = GetPawn();
	if (!IsValid(Pawn))
	{
		return;
	}

	Pawn->SetActorLocation(InSaveData.Position);
	Pawn->SetActorRotation(InSaveData.Rotation);

	APlayerController* PlayerController = GetPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetControlRotation(InSaveData.ControlRotation);
	}

	FMemoryReader Reader = FMemoryReader(InSaveData.CharacterByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Pawn->Serialize(Ar);

	TSet<UActorComponent*> Components = Pawn->GetComponents();
	for (UActorComponent* Component : Components)
	{
		if (!IsValid(Component) || !Component->Implements<USHSavableComponent>())
		{
			continue;
		}

		if (FSHComponentSaveData* ComponentData = InSaveData.ComponentsData.Find(Component->GetFName()))
		{
			ISHSavableComponent::Execute_LoadFromSaveData(Component, *ComponentData);
		}
	}
}
