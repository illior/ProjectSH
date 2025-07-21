// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SHAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY(LogSHAbilitySystem);

USHAbilitySystemComponent::USHAbilitySystemComponent(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{

}

bool USHAbilitySystemComponent::TryActivateAbilityByInputTag(const FGameplayTag& InputTag, bool bAllowRemoteActivation)
{
	if (!InputTag.IsValid())
	{
		return false;
	}

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.DynamicAbilityTags.HasTag(InputTag))
		{
			return TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
		}
	}

	return false;
}
