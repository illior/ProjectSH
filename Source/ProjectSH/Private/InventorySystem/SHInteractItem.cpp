// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHInteractItem.h"
#include "InventorySystem/SHInteractItemTarget.h"

ASHInteractItem::ASHInteractItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

ASHInteractItemTarget* ASHInteractItem::GetInteractItemTarget() const
{
	return Cast<ASHInteractItemTarget>(GetOwner());
}

void ASHInteractItem::StopInspectItem()
{
	ASHInteractItemTarget* InteractItemTarget = GetInteractItemTarget();
	if (IsValid(InteractItemTarget))
	{
		InteractItemTarget->StopInspectItem();
	}
}

void ASHInteractItem::SetConsumeInput(bool InValue)
{
	ASHInteractItemTarget* InteractItemTarget = GetInteractItemTarget();
	if (IsValid(InteractItemTarget))
	{
		InteractItemTarget->SetConsumeInput(InValue);
	}
}

void ASHInteractItem::BeginPlay()
{
	Super::BeginPlay();
}
