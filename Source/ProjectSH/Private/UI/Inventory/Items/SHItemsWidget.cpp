// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHItemsWidget.h"
#include "InteractionSystem/SHInteractableTargetActor.h"
#include "Components/SHInventoryComponent.h"
#include "InventorySystem/SHItemData.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "UI/Inventory/Items/SHSlotsContainerWidget.h"
#include "UI/Inventory/Items/SHItemsContainerWidget.h"
#include "UI/Inventory/Items/SHItemsCursorWidget.h"
#include "UI/Inventory/Items/SHItemDropDownMenuWidget.h"
#include "UI/Inventory/Items/SHInspectItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "UI/SHBaseMenuButtonWidget.h"

void USHItemsWidget::TryAddItem(USHItemData* InItemData)
{
	bConsumeAdditiveMove = true;

	ItemsCursor->SetTempItem(InItemData);
	ItemsCursor->SetActive(true);

	CurrentState = ESHItemInventoryState::MovingItem;
	OpenState = ESHInventoryOpenState::AddItem;

	SetCurrentUnselected();
	ItemName->SetText(InItemData->GetItemName());
	ItemDescription->SetText(InItemData->GetItemDescription());

	ItemsCursor->SetPosition(GetBestPosition(InItemData), false);
}

void USHItemsWidget::Show(bool bShowAnim)
{
	Super::Show(bShowAnim);

	SetCurrentUnselected();
	ItemsCursor->SetPosition(FIntPoint(0, 0), false);
	SetCurrentSelected();

	DropDownMenu->Close();
	CurrentState = ESHItemInventoryState::Select;
	OpenState = ESHInventoryOpenState::Default;
	bConsumeAdditiveMove = false;

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetCancelDescription(CloseDescription);

		InventoryWidget->SetMoveDescription(MoveDescription);
	}

	if (!GetOwningPlayer()->GetViewTarget()->IsA<APawn>())
	{
		OpenState = ESHInventoryOpenState::Interact;
		bConsumeAdditiveMove = true;
	}
}

void USHItemsWidget::Hide(bool bShowAnim)
{
	Super::Hide(bShowAnim);
}

void USHItemsWidget::Apply()
{
	if (OpenState == ESHInventoryOpenState::AddSlots)
	{
		TryCloseAddSlots();
		return;
	}

	switch (CurrentState)
	{
	case ESHItemInventoryState::Select:
		OpenDropDownMenu();
		break;
	case ESHItemInventoryState::DropDownMenu:
		DropDownMenu->Apply();
		break;
	case ESHItemInventoryState::MovingItem:
		StopMovingItem();
		break;
	case ESHItemInventoryState::InspectItem:
		ItemInspection->InputApply();
		break;
	case ESHItemInventoryState::BindWeapon:
		break;
	default:
		break;
	}
}

void USHItemsWidget::Cancel()
{
	if (OpenState == ESHInventoryOpenState::AddSlots)
	{
		TryCloseAddSlots();
		return;
	}

	switch (CurrentState)
	{
	case ESHItemInventoryState::Select:
		Close();
		break;
	case ESHItemInventoryState::DropDownMenu:
		CloseDropDownMenu();
		break;
	case ESHItemInventoryState::MovingItem:
		CancelMovingItem();
		break;
	case ESHItemInventoryState::InspectItem:
		ItemInspection->InputCancel();
		break;
	case ESHItemInventoryState::BindWeapon:
		break;
	default:
		break;
	}
}

void USHItemsWidget::Move(FVector2D Value, float ElapsedTime)
{
	if (CurrentState == ESHItemInventoryState::InspectItem)
	{
		ItemInspection->InputMove(Value, ElapsedTime);
	}
}

void USHItemsWidget::MoveWithFrequency(FVector2D Value)
{
	if (OpenState == ESHInventoryOpenState::AddSlots)
	{
		return;
	}

	switch (CurrentState)
	{
	case ESHItemInventoryState::Select:
		MoveCursor(FIntPoint(Value.X, -Value.Y));
		break;
	case ESHItemInventoryState::DropDownMenu:
		DropDownMenu->MoveVertical(-Value.Y);
		break;
	case ESHItemInventoryState::MovingItem:
		MoveCursor(FIntPoint(Value.X, -Value.Y));
		break;
	case ESHItemInventoryState::InspectItem:
		break;
	case ESHItemInventoryState::BindWeapon:
		break;
	default:
		break;
	}
}

void USHItemsWidget::AdditiveMove(FVector2D Value, float ElapsedTime)
{
	if (CurrentState == ESHItemInventoryState::InspectItem)
	{
		ItemInspection->InputAdditiveMove(Value, ElapsedTime);
	}
}

void USHItemsWidget::InspectItem()
{
	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());
	if (!IsValid(ItemData))
	{
		return;
	}

	CloseDropDownMenu();

	CurrentState = ESHItemInventoryState::InspectItem;
	bConsumeAdditiveMove = true;
	ItemInspection->StartInspect(ItemData);
}

void USHItemsWidget::EquipItem()
{
	UE_LOG(LogTemp, Display, TEXT("EquipItem"));
}

void USHItemsWidget::FastAccessItem()
{
	UE_LOG(LogTemp, Display, TEXT("FastAccessItem"));
}

void USHItemsWidget::CombineItem()
{
	UE_LOG(LogTemp, Display, TEXT("CombineItem"));

	CloseDropDownMenu();

	MovingItem = TWeakObjectPtr<USHItemData>(ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition()));

	ItemsCursor->SetActive(true);
	CurrentState = ESHItemInventoryState::MovingItem;
}

void USHItemsWidget::CancelMovingItem()
{
	if (OpenState == ESHInventoryOpenState::AddItem)
	{
		bConsumeAdditiveMove = false;

		ItemsCursor->ClearTempItem();
		ItemsCursor->SetActive(false);

		Close();
		return;
	}

	if (MovingItem.IsValid())
	{
		ItemsContainer->SetItemUnselected(MovingItem->GetPosition());
		MovingItem.Reset();
	}

	ItemsCursor->SetActive(false);
	SetCurrentSelected();

	CurrentState = ESHItemInventoryState::Select;
}

void USHItemsWidget::StopMovingItem()
{
	if (OpenState == ESHInventoryOpenState::AddItem)
	{
		USHInventoryComponent* CharacterInventory = GetCharacterInventory();
		if (CharacterInventory == nullptr)
		{
			return;
		}

		bool BSuccess = CharacterInventory->TryAddItem(ItemsCursor->GetTempItem());
		if (BSuccess)
		{
			bConsumeAdditiveMove = false;

			ItemsCursor->ClearTempItem();
			ItemsCursor->SetActive(false);

			Close();
		}

		return;
	}

	if (!MovingItem.IsValid())
	{
		ItemsCursor->SetActive(false);
		SetCurrentSelected();
		CurrentState = ESHItemInventoryState::Select;
		return;
	}

	FIntPoint PositionForItem = ItemsCursor->GetPosition();
	if (MovingItem->GetItemSize() == 2 && SlotsContainer->GetSlotState(PositionForItem + FIntPoint(1, 0)) == ESHSlotState::Block)
	{
		PositionForItem.X--;
	}

	bool BSuccess = MovingItem->SetPosition(PositionForItem);
	if (BSuccess)
	{
		SlotsContainer->UpdateSlots();

		ItemsCursor->SetActive(false);
		SetCurrentSelected();

		CurrentState = ESHItemInventoryState::Select;
	}
}

void USHItemsWidget::UseItem()
{
	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());
	if (!IsValid(ItemData))
	{
		return;
	}

	if (OpenState == ESHInventoryOpenState::Interact)
	{
		APlayerController* PlayerController = GetOwningPlayer();
		if (PlayerController != nullptr)
		{
			ASHInteractableTargetActor* InteractTarget = Cast<ASHInteractableTargetActor>(PlayerController->GetViewTarget());
			bool bShouldRemove = InteractTarget->UseItem(ItemData);
		
			if (bShouldRemove)
			{
				RemoveCurrentItem();
			}
		}

		return;
	}

	UE_LOG(LogTemp, Display, TEXT("UseItem"));
}

void USHItemsWidget::DropItem()
{
	UE_LOG(LogTemp, Display, TEXT("DropItem"));

	CloseDropDownMenu();

	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());
	if (ItemData != nullptr)
	{
		ItemData->DropItem();
	}

	SlotsContainer->UpdateSlots();
	SetCurrentSelected();
}

void USHItemsWidget::TryCloseAddSlots()
{
	if (SlotsContainer->CanClose())
	{
		bConsumeAdditiveMove = false;
		ItemsCursor->SetVisibility(ESlateVisibility::Visible);
		Close();
	}
}

void USHItemsWidget::StopInspectItem()
{
	CurrentState = ESHItemInventoryState::Select;
	bConsumeAdditiveMove = false;

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetCancelDescription(CloseDescription);

		InventoryWidget->SetMoveDescription(MoveDescription);
	}
}

void USHItemsWidget::InventoryInitialized()
{
	SlotsContainer->CreateSlots();
	ItemsContainer->CreateItems();

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemName->GetParent()->Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X,
			FMath::DivideAndRoundUp(SlotsContainer->GetCurrentSlotsCount(), SlotsContainer->GetInventorySize().X) * (SlotSize.Y + SlotPadding.Top + SlotPadding.Bottom)));
	}
}

void USHItemsWidget::ItemCreated(USHItemData* InItemData)
{
	ItemsContainer->CreateItem(InItemData);
	SlotsContainer->UpdateSlots();
}

void USHItemsWidget::SlotsIncremented(int32 InCount)
{
	bConsumeAdditiveMove = true;
	OpenState = ESHInventoryOpenState::AddSlots;

	ItemsCursor->SetVisibility(ESlateVisibility::Hidden);
	SetCurrentUnselected();
	ItemName->SetText(FText());
	ItemDescription->SetText(FText());

	SlotsContainer->AddSlots(InCount);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemName->GetParent()->Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetPosition(FVector2D(CanvasSlot->GetPosition().X,
			FMath::DivideAndRoundUp(SlotsContainer->GetCurrentSlotsCount(), SlotsContainer->GetInventorySize().X) * (SlotSize.Y + SlotPadding.Top + SlotPadding.Bottom)));
	}
}

void USHItemsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (SlotsContainer != nullptr)
	{
		SlotsContainer->SetSlotPadding(SlotPadding);
	}

	if (ItemsContainer != nullptr)
	{
		ItemsContainer->SetSlotPadding(SlotPadding);
	}

	if (ItemsCursor != nullptr)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemsCursor->Slot);
		if (CanvasSlot != nullptr)
		{
			CanvasSlot->SetSize(SlotSize);
		}

		ItemsCursor->SetSlotPadding(SlotPadding);
	}
}

void USHItemsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(SlotsContainer);
	check(ItemsContainer);
	check(ItemsCursor);
	check(DropDownMenu);
	check(ItemInspection);
	check(ItemName);
	check(ItemDescription);
	
	ItemInspection->OnCancelInspectItem.AddUObject(this, &USHItemsWidget::StopInspectItem);

	USHInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (CharacterInventory == nullptr)
	{
		return;
	}

	FVector2D ContainerSize = FVector2D(
		CharacterInventory->GetMaxSize().X * SlotSize.X + CharacterInventory->GetMaxSize().X * (SlotPadding.Right + SlotPadding.Left),
		CharacterInventory->GetMaxSize().Y * SlotSize.Y + CharacterInventory->GetMaxSize().Y * (SlotPadding.Top + SlotPadding.Bottom)
	);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SlotsContainer->Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetSize(ContainerSize);
	}

	CanvasSlot = Cast<UCanvasPanelSlot>(ItemsContainer->Slot);
	if (CanvasSlot != nullptr)
	{
		CanvasSlot->SetSize(ContainerSize);
	}

	DropDownMenu->GetInspectButtonSignature().AddDynamic(this, &USHItemsWidget::InspectItem);
	DropDownMenu->GetEquipButtonSignature().AddDynamic(this, &USHItemsWidget::EquipItem);
	DropDownMenu->GetFastAccessButtonSignature().AddDynamic(this, &USHItemsWidget::FastAccessItem);
	DropDownMenu->GetCombineButtonSignature().AddDynamic(this, &USHItemsWidget::CombineItem);
	DropDownMenu->GetUseButtonSignature().AddDynamic(this, &USHItemsWidget::UseItem);
	DropDownMenu->GetDropButtonSignature().AddDynamic(this, &USHItemsWidget::DropItem);

	if (CharacterInventory->IsInitialized())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USHItemsWidget::InventoryInitialized);
	}
	else
	{
		CharacterInventory->OnInventoryInitialized.AddUObject(this, &USHItemsWidget::InventoryInitialized);
	}

	CharacterInventory->OnItemCreated.AddUObject(this, &USHItemsWidget::ItemCreated);
	CharacterInventory->OnSlotsIncremented.AddUObject(this, &USHItemsWidget::SlotsIncremented);
}

FIntPoint USHItemsWidget::GetBestPosition(USHItemData* InItemData)
{
	if (InItemData == nullptr)
	{
		return FIntPoint(0, 0);
	}

	if (InItemData->GetItemCanStack() && !InItemData->IsFull())
	{
		TArray<USHItemData*> Items = ItemsContainer->GetAllItems();
		for (USHItemData* ItemData : Items)
		{
			if (ItemData->GetItem() == InItemData->GetItem() && !ItemData->IsFull())
			{
				return ItemData->GetPosition();
			}
		}
	}

	for (int32 Y = 0; Y < SlotsContainer->GetInventorySize().Y; Y++)
	{
		for (int32 X = 0; X < SlotsContainer->GetInventorySize().X; X++)
		{
			if (InItemData->GetItemSize() == 1 &&
				SlotsContainer->GetSlotState(FIntPoint(X, Y)) == ESHSlotState::Void)
			{
				return FIntPoint(X, Y);
			}

			if (InItemData->GetItemSize() == 2 &&
				SlotsContainer->GetSlotState(FIntPoint(X, Y)) == ESHSlotState::Void &&
				SlotsContainer->GetSlotState(FIntPoint(X + 1, Y)) == ESHSlotState::Void)
			{
				return FIntPoint(X, Y);
			}
		}
	}

	return FIntPoint(0, 0);
}

void USHItemsWidget::RemoveCurrentItem()
{
	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());

	USHInventoryComponent* CharacterInventory = GetCharacterInventory();
	if (IsValid(CharacterInventory))
	{
		CharacterInventory->RemoveItem(ItemData);
		SlotsContainer->UpdateSlots();

		SetCurrentSelected();
	}
}

void USHItemsWidget::OpenDropDownMenu()
{
	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());
	if (ItemData != nullptr)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DropDownMenu->Slot);
		if (CanvasSlot != nullptr)
		{
			FVector2D Position = FVector2D(ItemData->GetPosition().X * (SlotSize.X + SlotPadding.Left + SlotPadding.Right) + SlotSize.X,
				ItemData->GetPosition().Y * (SlotSize.Y + SlotPadding.Top + SlotPadding.Bottom));

			if (ItemData->GetItemSize() == 2)
			{
				Position += FVector2D(SlotSize.X + SlotPadding.Left + SlotPadding.Right, 0);
			}

			CanvasSlot->SetPosition(Position);
		}

		OpenState == ESHInventoryOpenState::Interact ? DropDownMenu->OpenToUse(ItemData) : DropDownMenu->Open(ItemData);
		CurrentState = ESHItemInventoryState::DropDownMenu;

		USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->SetApplyDescription(ApplyDescription);
			InventoryWidget->SetCancelDescription(CancelDescription);

			InventoryWidget->SetVerticalAndHorizontalDescription(VerticalDescription, FText());
		}
	}
}

void USHItemsWidget::CloseDropDownMenu()
{
	DropDownMenu->Close();
	CurrentState = ESHItemInventoryState::Select;

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetCancelDescription(CloseDescription);

		InventoryWidget->SetMoveDescription(MoveDescription);
	}
}

void USHItemsWidget::MoveCursor(FIntPoint Offset)
{
	FIntPoint TargetPosition = ItemsCursor->GetPosition();

	if (Offset.X != 0)
	{
		TargetPosition.X += Offset.X;
		if (SlotsContainer->GetSlotState(TargetPosition) == ESHSlotState::Block)
		{
			TargetPosition.X -= Offset.X;
		}
	}

	if (Offset.Y != 0)
	{
		TargetPosition.Y += Offset.Y;
		if (SlotsContainer->GetSlotState(TargetPosition) == ESHSlotState::Block)
		{
			TargetPosition.Y -= Offset.Y;
		}
	}

	if (ItemsCursor->GetPosition() != TargetPosition)
	{
		if (CurrentState == ESHItemInventoryState::Select)
		{
			SetCurrentUnselected();
		}



		ItemsCursor->SetPosition(TargetPosition, true, SlotsContainer->GetSlotState(TargetPosition + FIntPoint(1, 0)));
		if (CurrentState == ESHItemInventoryState::Select)
		{
			SetCurrentSelected();
		}
	}
}

void USHItemsWidget::SetCurrentSelected()
{
	USHItemData* ItemData = ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition());
	if (IsValid(ItemData))
	{
		ItemsContainer->SetItemSelected(ItemsCursor->GetPosition());

		ItemName->SetText(ItemData->GetItemName());
		ItemDescription->SetText(ItemData->GetItemDescription());
	}
	else
	{
		SlotsContainer->SetSlotSelected(ItemsCursor->GetPosition());

		ItemName->SetText(FText());
		ItemDescription->SetText(FText());
	}

	USHInventoryWidget* InventoryWidget = Cast<USHInventoryWidget>(GetOuter());
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->SetApplyDescription(IsValid(ItemData) ? ApplyDescription : FText());
	}
}

void USHItemsWidget::SetCurrentUnselected()
{
	if (ItemsContainer->GetItemDataByPosition(ItemsCursor->GetPosition()) != nullptr)
	{
		ItemsContainer->SetItemUnselected(ItemsCursor->GetPosition());
	}
	else
	{
		SlotsContainer->SetSlotUnselected(ItemsCursor->GetPosition());
	}
}
