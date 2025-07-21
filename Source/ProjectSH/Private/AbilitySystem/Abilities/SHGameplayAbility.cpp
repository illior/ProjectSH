// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/SHGameplayAbility.h"
#include "Components/SHAbilitySystemComponent.h"
#include "Player/SHPlayerController.h"
#include "Player/SHCharacter.h"

USHAbilitySystemComponent* USHGameplayAbility::GetSHAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<USHAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ASHPlayerController* USHGameplayAbility::GetSHPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASHPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

ASHCharacter* USHGameplayAbility::GetSHCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ASHCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}
