// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHMoveItemAsyncAction.h"
#include "InventorySystem/SHInteractItemTarget.h"
#include "InventorySystem/SHInteractItem.h"
#include "Components/ArrowComponent.h"

USHMoveItemAsyncAction* USHMoveItemAsyncAction::MoveItemToTarget(UObject* WorldContextObject, ASHInteractItemTarget* ItemTarget, FTransform Position, float Time)
{
	USHMoveItemAsyncAction* Action = NewObject<USHMoveItemAsyncAction>();
	Action->ItemTarget = ItemTarget;
	Action->Position = Position;
	Action->Time = Time;

	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void USHMoveItemAsyncAction::Activate()
{
	Super::Activate();

	if (!ItemTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemTarget nullptr OnCompleted.Broadcast()"));

		Complete();
		return;
	}

	ItemTarget->OnMoveComplete.AddDynamic(this, &USHMoveItemAsyncAction::Complete);
	ItemTarget->MoveItemToPosition(Position, Time);
}

void USHMoveItemAsyncAction::Complete()
{
	OnCompleted.Broadcast();

	ItemTarget->OnMoveComplete.RemoveAll(this);
	SetReadyToDestroy();
}
