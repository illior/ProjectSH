// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SHInventoryComponent.h"
#include "InventorySystem/SHCraftingList.h"
#include "InventorySystem/Items/SHBaseItem.h"
#include "InventorySystem/Items/SHBaseWeaponItem.h"
#include "InventorySystem/SHWeaponData.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

USHInventoryComponent::USHInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

FSHComponentSaveData USHInventoryComponent::GetSaveData_Implementation()
{
	FSHComponentSaveData Record = FSHComponentSaveData();

	Record.ComponentName = GetFName();

	SerializeItems();

	FMemoryWriter Writer = FMemoryWriter(Record.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);

	return Record;
}

void USHInventoryComponent::LoadFromSaveData_Implementation(FSHComponentSaveData InRecord)
{
	FMemoryReader Reader = FMemoryReader(InRecord.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);

	UnserializeItems();
}

ESHSlotState USHInventoryComponent::GetSlotState(FIntPoint InPosition) const
{
	if (SlotsMatrix == nullptr ||
		InPosition.X < 0 || InPosition.X >= MaxSize.X ||
		InPosition.Y < 0 || InPosition.Y >= MaxSize.Y)
	{
		return ESHSlotState::Block;
	}

	return SlotsMatrix[InPosition.Y][InPosition.X];
}

TIndexedContainerIterator<const TArray<TObjectPtr<USHItemData>, FDefaultAllocator>, TObjectPtr<USHItemData>const, int32> USHInventoryComponent::GetItemsIterator() const
{
	return Items.CreateConstIterator();
}

TIndexedContainerIterator<const TArray<FSHRecord, FDefaultAllocator>, FSHRecord const, int32> USHInventoryComponent::GetRecordsIterator() const
{
	return Records.CreateConstIterator();
}

USHItemData* USHInventoryComponent::GetItemByPosition(FIntPoint InPosition) const
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i]->ContainsPoint(InPosition))
		{
			return Items[i];
		}
	}

	return nullptr;
}

bool USHInventoryComponent::CanSetItemPosition(USHItemData* InItemData, FIntPoint InPostion) const
{
	if (!Items.Contains(InItemData))
	{
		return false;
	}

	if (InItemData->GetItemSize() == 1)
	{
		if (GetSlotState(InPostion) == ESHSlotState::Void)
		{
			return true;
		}

		USHItemData* ItemInPosition = GetItemByPosition(InPostion);
		if (ItemInPosition != nullptr && ItemInPosition->GetItemSize() == 1)
		{
			return true;
		}
	}
	else
	{
		if (GetSlotState(InPostion) == ESHSlotState::Void && GetSlotState(InPostion + FIntPoint(1, 0)) == ESHSlotState::Void)
		{
			return true;
		}

		USHItemData* ItemInPosition = GetItemByPosition(InPostion);
		if (ItemInPosition != nullptr && ItemInPosition->GetItemSize() == 2 && InItemData->GetPosition() == ItemInPosition->GetPosition())
		{
			return true;
		}
	}

	return false;
}

bool USHInventoryComponent::CanCombineItems(USHBaseItem* InItem1, USHBaseItem* InItem2) const
{
	return InItem1 != nullptr && InItem1->GetCanStack() && InItem1 == InItem2;
}

bool USHInventoryComponent::CanCraftItems(USHBaseItem* InItem1, USHBaseItem* InItem2) const
{
	FSHCraftRecipe CraftRecipe;
	return IsValid(CraftingList) ? CraftingList->GetCraftRecipe(InItem1, InItem2, CraftRecipe) : false;
}

bool USHInventoryComponent::TryAddItem(USHItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		return false;
	}

	USHItemData* ItemInPosition = GetItemByPosition(InItemData->GetPosition());
	if (ItemInPosition != nullptr)
	{
		if (CanCombineItems(ItemInPosition->GetItem(), InItemData->GetItem()))
		{
			CombineItems(ItemInPosition, InItemData);

			return InItemData->GetCount() == 0;
		}

		if (CanCraftItems(ItemInPosition->GetItem(), InItemData->GetItem()))
		{
			CraftItems(InItemData, ItemInPosition);

			return true;
		}

		return false;
	}

	if (InItemData->GetItemSize() == 1 && GetSlotState(InItemData->GetPosition()) == ESHSlotState::Void)
	{
		SetSlotState(InItemData->GetPosition(), ESHSlotState::Item);

		Items.Add(InItemData);
		InItemData->Rename(nullptr, this);

		InItemData->OnPositionChanged.AddDynamic(this, &USHInventoryComponent::ItemPositionChanged);
		InItemData->OnDropped.AddDynamic(this, &USHInventoryComponent::ItemDropped);

		OnItemCreated.Broadcast(InItemData);

		return true;
	}

	if (InItemData->GetItemSize() == 2 && GetSlotState(InItemData->GetPosition()) == ESHSlotState::Void && GetSlotState(InItemData->GetPosition() + FIntPoint(1, 0)) == ESHSlotState::Void)
	{
		SetSlotState(InItemData->GetPosition(), ESHSlotState::Item);
		SetSlotState(InItemData->GetPosition() + FIntPoint(1, 0), ESHSlotState::Item);

		InItemData->Rename(nullptr, this);
		Items.Add(InItemData);

		InItemData->OnPositionChanged.AddDynamic(this, &USHInventoryComponent::ItemPositionChanged);
		InItemData->OnDropped.AddDynamic(this, &USHInventoryComponent::ItemDropped);

		OnItemCreated.Broadcast(InItemData);

		return true;
	}

	return false;
}

void USHInventoryComponent::RemoveItem(USHItemData* InItemData)
{
	if (Items.Contains(InItemData))
	{
		InItemData->OnPositionChanged.RemoveDynamic(this, &USHInventoryComponent::ItemPositionChanged);
		InItemData->OnDropped.RemoveDynamic(this, &USHInventoryComponent::ItemDropped);

		Items.Remove(InItemData);
		SetSlotState(InItemData->GetPosition(), ESHSlotState::Void);
		if (InItemData->GetItemSize() == 2)
		{
			SetSlotState(InItemData->GetPosition() + FIntPoint(1, 0), ESHSlotState::Void);
		}

		OnItemRemoved.Broadcast(InItemData);
	}
}

void USHInventoryComponent::AddSlots(int32 InCount)
{
	if (InCount <= 0 || CurrentSlotsCount == MaxSize.X * MaxSize.Y)
	{
		return;
	}

	int32 DeltaSlots = FMath::Min(InCount, MaxSize.X * MaxSize.Y - CurrentSlotsCount);
	for (int32 i = CurrentSlotsCount; i < CurrentSlotsCount + DeltaSlots; i++)
	{
		SetSlotState(FIntPoint(i % MaxSize.X, i / MaxSize.X), ESHSlotState::Void);
	}

	CurrentSlotsCount += DeltaSlots;
	OnSlotsIncremented.Broadcast(DeltaSlots);
}

void USHInventoryComponent::AddRecord(FSHRecord InRecord)
{
	Records.Add(InRecord);

	OnRecordAdded.Broadcast(InRecord);
}

void USHInventoryComponent::PrintMatrix()
{
	if (SlotsMatrix == nullptr)
	{
		UE_LOG(LogInventoryComponent, Warning, TEXT("Matrix nullptr"));

		return;
	}

	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		FString Str = FString(TEXT(""));

		for (int32 j = 0; j < MaxSize.X; j++)
		{
			Str += UEnum::GetValueAsString(SlotsMatrix[i][j]) + TEXT(" ");
		}

		UE_LOG(LogInventoryComponent, Display, TEXT("%s"), *Str);
	}
}

void USHInventoryComponent::ItemPositionChanged(FIntPoint OldPosition, FIntPoint NewPosition)
{
	USHItemData* MovedItem = GetItemByPosition(OldPosition);
	if (MovedItem == nullptr)
	{
		return;
	}

	if (MovedItem->GetItemSize() == 1)
	{
		if (GetSlotState(NewPosition) == ESHSlotState::Void)
		{
			SetSlotState(OldPosition, ESHSlotState::Void);
			SetSlotState(NewPosition, ESHSlotState::Item);

			return;
		}

		USHItemData* SwappedItem = GetItemByPosition(NewPosition);
		if (SwappedItem == nullptr)
		{
			return;
		}

		if (CanCombineItems(MovedItem->GetItem(), SwappedItem->GetItem()))
		{
			CombineItems(MovedItem, SwappedItem);

			return;
		}

		if (CanCraftItems(MovedItem->GetItem(), SwappedItem->GetItem()))
		{
			CraftItems(MovedItem, SwappedItem);

			return;
		}
	
		SetSlotState(OldPosition, ESHSlotState::Void);
		SwappedItem->SetPosition(OldPosition);
		SetSlotState(NewPosition, ESHSlotState::Item);
	}
	else
	{
		if (GetSlotState(NewPosition) == ESHSlotState::Void && GetSlotState(NewPosition + FIntPoint(1, 0)) == ESHSlotState::Void)
		{
			SetSlotState(OldPosition, ESHSlotState::Void);
			SetSlotState(OldPosition + FIntPoint(1, 0), ESHSlotState::Void);
			SetSlotState(NewPosition, ESHSlotState::Item);
			SetSlotState(NewPosition + FIntPoint(1, 0), ESHSlotState::Item);

			return;
		}

		if (OldPosition + FIntPoint(1, 0) == NewPosition && GetSlotState(NewPosition + FIntPoint(1, 0)) == ESHSlotState::Void)
		{
			SetSlotState(OldPosition, ESHSlotState::Void);
			SetSlotState(NewPosition + FIntPoint(1, 0), ESHSlotState::Item);

			return;
		}

		USHItemData* SwappedItem = GetItemByPosition(NewPosition);
		if (SwappedItem == nullptr)
		{
			return;
		}

		SetSlotState(OldPosition, ESHSlotState::Void);
		SetSlotState(OldPosition + FIntPoint(1, 0), ESHSlotState::Void);
		SwappedItem->SetPosition(OldPosition);
		SetSlotState(NewPosition, ESHSlotState::Item);
		SetSlotState(NewPosition + FIntPoint(1, 0), ESHSlotState::Item);
	}
}

void USHInventoryComponent::ItemDropped(USHItemData* InItemData)
{
	Items.Remove(InItemData);

	SetSlotState(InItemData->GetPosition(), ESHSlotState::Void);
	if (InItemData->GetItemSize() == 2)
	{
		SetSlotState(InItemData->GetPosition() + FIntPoint(1, 0), ESHSlotState::Void);
	}
}

void USHInventoryComponent::SetSlotState(const FIntPoint& InPosition, const ESHSlotState& InState)
{
	if (SlotsMatrix == nullptr ||
		InPosition.X < 0 || InPosition.X >= MaxSize.X ||
		InPosition.Y < 0 || InPosition.Y >= MaxSize.Y)
	{
		return;
	}

	SlotsMatrix[InPosition.Y][InPosition.X] = InState;
}

void USHInventoryComponent::CombineItems(USHItemData* InItemData1, USHItemData* InItemData2)
{
	if ((InItemData1->GetCount() + InItemData2->GetCount()) > InItemData1->GetItemMaxStackAmount())
	{
		InItemData2->SetCount((InItemData1->GetCount() + InItemData2->GetCount()) - InItemData1->GetItemMaxStackAmount());
		InItemData1->SetCount(InItemData2->GetItemMaxStackAmount());

		if (InItemData2->GetOuter() == this)
		{
			FIntPoint CachedPosition = InItemData2->GetPosition();

			SetSlotState(InItemData1->GetPosition(), ESHSlotState::Void);
			InItemData2->SetPosition(InItemData1->GetPosition());
			SetSlotState(CachedPosition, ESHSlotState::Item);
		}
	}
	else
	{
		if (InItemData1->GetOuter() == InItemData2->GetOuter())
		{
			InItemData2->SetCount(InItemData1->GetCount() + InItemData2->GetCount());
			InItemData1->DropItem();
		}
		else
		{
			InItemData1->SetCount(InItemData1->GetCount() + InItemData2->GetCount());
			InItemData2->SetCount(0);
		}
	}
}

void USHInventoryComponent::CraftItems(USHItemData* InItemData1, USHItemData* InItemData2)
{
	FSHCraftRecipe CraftRecipe;
	if (IsValid(CraftingList) && IsValid(InItemData1) && IsValid(InItemData2) && CraftingList->GetCraftRecipe(InItemData1->GetItem(), InItemData2->GetItem(), CraftRecipe))
	{
		USHItemData* NewItemData = CraftRecipe.Result->IsA<USHBaseWeaponItem>() ? NewObject<USHWeaponData>(this) : NewObject<USHItemData>(this);
		NewItemData->Initialize(CraftRecipe.Result, CraftRecipe.Count, InItemData2->GetPosition());

		NewItemData->OnPositionChanged.AddDynamic(this, &USHInventoryComponent::ItemPositionChanged);
		NewItemData->OnDropped.AddDynamic(this, &USHInventoryComponent::ItemDropped);

		InItemData1->DropItem();
		InItemData2->DropItem();

		Items.Add(NewItemData);
		SetSlotState(NewItemData->GetPosition(), ESHSlotState::Item);
		if (NewItemData->GetItemSize() == 2)
		{
			SetSlotState(NewItemData->GetPosition() + FIntPoint(1, 0), ESHSlotState::Item);
		}

		OnItemCreated.Broadcast(NewItemData);
	}
}

void USHInventoryComponent::Initialize()
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		Items[i]->OnPositionChanged.AddDynamic(this, &USHInventoryComponent::ItemPositionChanged);
		Items[i]->OnDropped.AddDynamic(this, &USHInventoryComponent::ItemDropped);

		SetSlotState(Items[i]->GetPosition(), ESHSlotState::Item);

		if (Items[i]->GetItemSize() == 2)
		{
			SetSlotState(Items[i]->GetPosition() + FIntPoint(1, 0), ESHSlotState::Item);
		}
	}

	OnInventoryInitialized.Broadcast();
	bInitialized = true;

	UE_LOG(LogInventoryComponent, Display, TEXT("Inventory Component Initialized"));
}

void USHInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateMatrix();
	Initialize();
}

void USHInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DestroyMatrix();
}

void USHInventoryComponent::CreateMatrix()
{
	SlotsMatrix = new ESHSlotState * [MaxSize.Y];

	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		SlotsMatrix[i] = new ESHSlotState[MaxSize.X];

		for (int32 j = 0; j < MaxSize.X; j++)
		{
			if ((i * MaxSize.X + j) < CurrentSlotsCount)
			{
				SlotsMatrix[i][j] = ESHSlotState::Void;
				continue;
			}

			SlotsMatrix[i][j] = ESHSlotState::Block;
		}
	}
}

void USHInventoryComponent::DestroyMatrix()
{
	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		delete[] SlotsMatrix[i];
	}
	delete[] SlotsMatrix;

	SlotsMatrix = nullptr;
}

void USHInventoryComponent::SerializeItems()
{
	ItemsByteData.Empty();
	for (USHItemData* ItemData : Items)
	{
		FSHItemBytesData ItemByteData = FSHItemBytesData();

		FMemoryWriter Writer = FMemoryWriter(ItemByteData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(Writer, false);
		Ar.ArIsSaveGame = true;
		Ar.ArNoDelta = true;

		ItemData->Serialize(Ar);

		ItemByteData.bIsWeapon = ItemData->GetItem()->IsA<USHBaseWeaponItem>();

		ItemsByteData.Add(ItemByteData);
	}
}

void USHInventoryComponent::UnserializeItems()
{
	Items.Empty();
	for (FSHItemBytesData ItemByteData : ItemsByteData)
	{
		USHItemData* ItemData = ItemByteData.bIsWeapon ? NewObject<USHWeaponData>(this) : NewObject<USHItemData>(this);

		FMemoryReader Reader = FMemoryReader(ItemByteData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(Reader, false);
		Ar.ArIsSaveGame = true;
		Ar.ArNoDelta = true;

		ItemData->Serialize(Ar);
		Items.Add(ItemData);
	}
}
