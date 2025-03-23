// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SHUsableItem.generated.h"

UINTERFACE(MinimalAPI)
class USHUsableItem : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSH_API ISHUsableItem
{
	GENERATED_BODY()


public:
	virtual bool CanUse(APawn* InPawn) const;

	virtual void Use(APawn* InPawn);
};
