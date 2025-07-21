// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHTakeableItem.h"
#include "Components/SHInteractWidgetComponent.h"
#include "Player/SHCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "InventorySystem/SHItemData.h"
#include "SHGameModeBase.h"

ASHTakeableItem::ASHTakeableItem()
{
	bIsReusable = false;
	CooldownTime = 0.5f;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));
}

FSHActorSaveData ASHTakeableItem::GetSaveData_Implementation()
{
	FMemoryWriter Writer = FMemoryWriter(ItemByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	ItemData->Serialize(Ar);

	return Super::GetSaveData_Implementation();
}

void ASHTakeableItem::LoadFromSaveData_Implementation(FSHActorSaveData InRecord)
{
	Super::LoadFromSaveData_Implementation(InRecord);

	FMemoryReader Reader = FMemoryReader(ItemByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	ItemData->Serialize(Ar);
}

void ASHTakeableItem::Interact(ASHCharacter* InCharacter)
{
	OnInteracted.Broadcast(this, InCharacter);

	bIsEnabled = false;
	GetWidget()->bShouldShow = false;

	InCharacter->AddItem(ItemData);

	ASHGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASHGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->OnPauseStateChanged.AddDynamic(this, &ASHTakeableItem::PauseStateChanged);
	}
}

void ASHTakeableItem::PauseStateChanged(bool bIsPaused)
{
	if (!IsValid(ItemData) || ItemData->GetCount() == 0 || ItemData->GetOuter() != this)
	{
		Destroy();
		return;
	}

	Character.Reset();
	ASHGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASHGameModeBase>();
	if (GameMode != nullptr)
	{
		GameMode->OnPauseStateChanged.RemoveDynamic(this, &ASHTakeableItem::PauseStateChanged);
	}

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "SetIsEnabled", true);

	GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
}
