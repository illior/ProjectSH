// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "SHAbilitySet.generated.h"

class USHAbilitySystemComponent;
class USHGameplayAbility;
class UGameplayEffect;
class UAttributeSet;

USTRUCT(BlueprintType)
struct FSHAbilitySet_GameplayAbility
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USHGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FSHAbilitySet_GameplayEffect
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FSHAbilitySet_AttributeSet
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

USTRUCT(BlueprintType)
struct FSHAbilitySetGrantedHandles
{
	GENERATED_BODY()


public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(USHAbilitySystemComponent* AbilitySystem);
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTSH_API USHAbilitySet : public UDataAsset
{
	GENERATED_BODY()


public:
	void GiveToAbilitySystem(USHAbilitySystemComponent* AbilitySystem, FSHAbilitySetGrantedHandles* GrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FSHAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FSHAbilitySet_GameplayEffect> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
	TArray<FSHAbilitySet_AttributeSet> GrantedAttributes;
};
