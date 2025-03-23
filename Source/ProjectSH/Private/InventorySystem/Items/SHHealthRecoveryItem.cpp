// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/Items/SHHealthRecoveryItem.h"
#include "Components/SHHealthComponent.h"

bool USHHealthRecoveryItem::CanUse(APawn* InPawn) const
{
	if (InPawn == nullptr)
	{
		return false;
	}

	USHHealthComponent* CharacterHealth = InPawn->GetComponentByClass<USHHealthComponent>();
	if (CharacterHealth == nullptr)
	{
		return false;
	}

	return false;
}

void USHHealthRecoveryItem::Use(APawn* InPawn)
{

}
