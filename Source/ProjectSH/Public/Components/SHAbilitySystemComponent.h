// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SHAbilitySystemComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSHAbilitySystem, Log, All);

UCLASS()
class PROJECTSH_API USHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


public:
	USHAbilitySystemComponent(const FObjectInitializer& ObjInit);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByInputTag(const FGameplayTag& InputTag, bool bAllowRemoteActivation = true);
};
