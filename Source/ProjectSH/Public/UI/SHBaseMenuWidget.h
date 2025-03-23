// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHBaseMenuWidget.generated.h"

class USHBaseMenuButtonWidget;

UCLASS()
class PROJECTSH_API USHBaseMenuWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void Apply();
	virtual void MoveVertical(int32 Offset);

protected:
	UPROPERTY()
	TArray<USHBaseMenuButtonWidget*> Buttons;

	int32 CurrentIndex = 0;

	virtual void NativeOnInitialized() override;
};
