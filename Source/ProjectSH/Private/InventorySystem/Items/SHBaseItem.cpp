// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/Items/SHBaseItem.h"

FString USHBaseItem::ToString() const
{
	return FString::Printf(TEXT("Name: %s, Description: %s, CanStack: %s, MaxStackAmount: %i, Size: %i"),
		*(DisplayName.ToString()),
		*(DisplayDescription.ToString()),
		bCanStack ? TEXT("True") : TEXT("False"),
		MaxStackAmount,
		Size);
}

#if WITH_EDITOR
void USHBaseItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!bCanStack)
	{
		MaxStackAmount = 1;
	}
}
#endif
