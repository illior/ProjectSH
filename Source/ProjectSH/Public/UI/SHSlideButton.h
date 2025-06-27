// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SHSlideButton.generated.h"

UINTERFACE(MinimalAPI)
class USHSlideButton : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSH_API ISHSlideButton
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH")
	void HorizontalMove(int32 Offset);
	virtual void HorizontalMove_Implementation(int32 Offset);

};
