// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/SHAbilitySet.h"
#include "Components/SHAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/SHGameplayAbility.h"

void FSHAbilitySetGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FSHAbilitySetGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FSHAbilitySetGrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (IsValid(Set))
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FSHAbilitySetGrantedHandles::TakeFromAbilitySystem(USHAbilitySystemComponent* AbilitySystem)
{
	if (!IsValid(AbilitySystem) || !AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		AbilitySystem->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

void USHAbilitySet::GiveToAbilitySystem(USHAbilitySystemComponent* AbilitySystem, FSHAbilitySetGrantedHandles* GrantedHandles, UObject* SourceObject) const
{
	if (!IsValid(AbilitySystem) || !AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FSHAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogSHAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		USHGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<USHGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystem->GiveAbility(AbilitySpec);

		if (GrantedHandles != nullptr)
		{
			GrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FSHAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogSHAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, AbilitySystem->MakeEffectContext());

		if (GrantedHandles != nullptr)
		{
			GrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FSHAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogSHAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(AbilitySystem->GetOwner(), SetToGrant.AttributeSet);
		AbilitySystem->AddAttributeSetSubobject(NewSet);

		if (GrantedHandles != nullptr)
		{
			GrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
