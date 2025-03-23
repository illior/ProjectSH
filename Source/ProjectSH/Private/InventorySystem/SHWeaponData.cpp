// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHWeaponData.h"
#include "InventorySystem/Items/SHBaseWeaponItem.h"

int32 USHWeaponData::GetItemMaxAmmoInClip() const
{
	USHBaseWeaponItem* WeaponItem = GetItem<USHBaseWeaponItem>();
	if (WeaponItem != nullptr)
	{
		return WeaponItem->GetMaxAmmoInClip();
	}

	return 1;
}

ESHAmmoType USHWeaponData::GetItemAmmoType() const
{
	USHBaseWeaponItem* WeaponItem = GetItem<USHBaseWeaponItem>();
	if (WeaponItem != nullptr)
	{
		return WeaponItem->GetAmmoType();
	}

	return ESHAmmoType::Pistol;
}

void USHWeaponData::Initialize(USHBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (InItem == nullptr)
	{
		return;
	}

	checkf(InItem->IsA<USHBaseWeaponItem>(), TEXT("InItem must be %s type"), *USHBaseWeaponItem::StaticClass()->GetFName().ToString());

	Item = InItem;
	Count = 1;
	Position = InPosition;
	CurrentAmmoInClip = FMath::Clamp(InCount, 0, GetItemMaxAmmoInClip());
}

bool USHWeaponData::IsFull() const
{
	return GetItemMaxAmmoInClip() == CurrentAmmoInClip;
}

bool USHWeaponData::CanDrop() const
{
	return false;
}

#if WITH_EDITOR
void USHWeaponData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	USHBaseWeaponItem* WeaponItem = Cast<USHBaseWeaponItem>(Item);
	if (WeaponItem != nullptr)
	{
		CurrentAmmoInClip = FMath::Clamp(CurrentAmmoInClip, 0, WeaponItem->GetMaxAmmoInClip());
	}
	else
	{
		Item = nullptr;
	}
}
#endif
