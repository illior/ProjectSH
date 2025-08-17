// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHItemData.h"
#include "Components/SHInventoryComponent.h"
#include "InventorySystem/Items/SHBaseItem.h"

FText USHItemData::GetItemName() const
{
	if (Item != nullptr)
	{
		return Item->GetDisplayName();
	}

	return FText();
}

FText USHItemData::GetItemDescription() const
{
	if (Item != nullptr)
	{
		return Item->GetDisplayDescription();
	}

	return FText();
}

bool USHItemData::GetItemCanStack() const
{
	if (Item != nullptr)
	{
		return Item->GetCanStack();
	}

	return false;
}

int32 USHItemData::GetItemMaxStackAmount() const
{
	if (Item != nullptr)
	{
		return Item->GetMaxStackAmount();
	}

	return 1;
}

int32 USHItemData::GetItemSize() const
{
	if (Item != nullptr)
	{
		return Item->GetSize();
	}

	return 1;
}

UTexture2D* USHItemData::GetItemIcon() const
{
	if (Item != nullptr)
	{
		return Item->GetIcon();
	}

	return nullptr;
}

TSoftClassPtr<ASHInteractItem> USHItemData::GetInteractItemClass() const
{
	if (Item != nullptr)
	{
		return Item->GetInteractItemClass();
	}

	return nullptr;
}

FString USHItemData::ToString() const
{
	if (Item == nullptr)
	{
		return FString(TEXT("Item nullptr"));
	}

	return FString::Printf(TEXT("%s, Count: %i, Position: %s"), *Item->ToString(), Count, *(Position.ToString()));
}

void USHItemData::Initialize(USHBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (InItem == nullptr)
	{
		return;
	}

	Item = InItem;
	Count = FMath::Clamp(InCount, 0, Item->GetMaxStackAmount());
	Position = InPosition;
}

bool USHItemData::SetPosition(FIntPoint InPosition)
{
	if (Position == InPosition)
	{
		return true;
	}

	USHInventoryComponent* InventoryComponent = Cast<USHInventoryComponent>(GetOuter());
	if (InventoryComponent == nullptr)
	{
		OnPositionChanged.Broadcast(Position, InPosition);
		Position = InPosition;

		return true;
	}

	if (InventoryComponent->CanSetItemPosition(this, InPosition))
	{
		OnPositionChanged.Broadcast(Position, InPosition);
		Position = InPosition;

		return true;
	}

	return false;
}

void USHItemData::SetCount(int32 InCount)
{
	if (InCount == Count)
	{
		return;
	}

	Count = FMath::Clamp(InCount, 0, GetItemMaxStackAmount());
	OnCountChanged.Broadcast(Count);
}

void USHItemData::DropItem()
{
	if (!CanDrop())
	{
		return;
	}

	OnDropped.Broadcast(this);

	OnPositionChanged.Clear();
	OnDropped.Clear();

	MarkAsGarbage();
}

bool USHItemData::ContainsPoint(FIntPoint InPosition) const
{
	return InPosition == Position || (GetItemSize() == 2 && FIntPoint(Position.X + 1, Position.Y) == InPosition);
}

bool USHItemData::IsFull() const
{
	if (Item != nullptr)
	{
		return Count == Item->GetMaxStackAmount();
	}

	return false;
}

bool USHItemData::CanDrop() const
{
	return true;
}

#if WITH_EDITOR
void USHItemData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (Item != nullptr)
	{
		Count = FMath::Clamp(Count, 1, Item->GetMaxStackAmount());
	}

	const USHInventoryComponent* InventoryComponent = Cast<USHInventoryComponent>(GetOuter());
	if (InventoryComponent != nullptr)
	{
		Position.X = FMath::Clamp(Position.X, 0, InventoryComponent->GetMaxSize().X - 1);
		Position.Y = FMath::Clamp(Position.Y, 0, InventoryComponent->GetMaxSize().Y - 1);
	}
}
#endif
