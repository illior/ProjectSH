// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/SHAbilitySet.h"
#include "SHCoreTypes.h"
#include "SHPlayerState.generated.h"

class USHAbilitySystemComponent;

UCLASS()
class PROJECTSH_API ASHPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	ASHPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FSHPlayerSaveData GetPlayerData();

	void LoadState(FSHPlayerSaveData InSaveData);
	void LoadCharacter(FSHPlayerSaveData InSaveData);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|AbilitySystem")
	void AddAbilitySet(USHAbilitySet* InAbilitySet);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|AbilitySystem")
	void RemoveAbilitySet(USHAbilitySet* InAbilitySet);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|AbilitySystem")
	void ClearAllAbilitySets();

protected:
	UPROPERTY()
	TMap<USHAbilitySet*, FSHAbilitySetGrantedHandles> GrantedAbilitySets;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USHAbilitySystemComponent> AbilitySystemComponent;
};
