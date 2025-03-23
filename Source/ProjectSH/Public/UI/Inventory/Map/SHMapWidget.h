// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SHSubInventoryWidget.h"
#include "SHMapWidget.generated.h"

UCLASS()
class PROJECTSH_API USHMapWidget : public USHSubInventoryWidget
{
	GENERATED_BODY()


public:
	virtual void Show(bool bShowAnim) override;

	virtual void Apply();
	virtual void Cancel();

	virtual void MoveWithFrequency(FVector2D Value);
};
