// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SHGameplayAbility.generated.h"

//FAbilitySystemTweaks::ClearAbilityTimers = 0;

class USHAbilitySystemComponent;
class ASHPlayerController;
class ASHCharacter;

UCLASS(Abstract)
class PROJECTSH_API USHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|Ability")
	USHAbilitySystemComponent* GetSHAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|Ability")
	ASHPlayerController* GetSHPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|Ability")
	ASHCharacter* GetSHCharacterFromActorInfo() const;
};
