// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/Interfaces/SHUsableItem.h"

bool ISHUsableItem::CanUse(APawn* InPawn) const
{
	return false;
}

void ISHUsableItem::Use(APawn* InPawn)
{

}
