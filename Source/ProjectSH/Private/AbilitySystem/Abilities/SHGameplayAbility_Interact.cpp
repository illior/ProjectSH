// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/SHGameplayAbility_Interact.h"
#include "AbilitySystem/SHGameplayTags.h"
#include "InteractionSystem/SHInteractableActor.h"
#include "Player/SHCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SHTickManagerComponent.h"

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FSHInteractData& Thing)
{
	return GetTypeHash(Thing.InteractActor);
}
#endif

USHGameplayAbility_Interact::USHGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags = FGameplayTagContainer(SHGameplayTags::Gameplay_Ability_Interact);
}

bool USHGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) && PreviousBestIndex != INDEX_NONE;
}

void USHGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		const bool bReplicateEndAbility = true;
		const bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}

	ASHCharacter* Character = GetSHCharacterFromActorInfo();

	PreviousClosestActors[PreviousBestIndex].InteractActor->Interact(Character);

	PreviousClosestActors.RemoveAt(PreviousBestIndex);
	PreviousBestIndex = INDEX_NONE;
	bIsActive = true;
}

void USHGameplayAbility_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USHGameplayAbility_Interact::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	InteractDistanceSqr = FMath::Square(InteractDistance);
	InteractSearchDistanceSqr = FMath::Square(InteractSearchDistance);

	TickComponent = FindOrCreateTickManager();
	TickDelegateHandle = TickComponent->OnTick.AddUObject(this, &USHGameplayAbility_Interact::AbilityTick);
}

void USHGameplayAbility_Interact::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	for (FSHInteractData PrevInteractData : PreviousClosestActors)
	{
		if (IsValid(PrevInteractData.InteractActor))
		{
			PrevInteractData.InteractActor->StopCanInteract(GetSHCharacterFromActorInfo());
		}
	}

	if (IsValid(TickComponent) && TickDelegateHandle.IsValid())
	{
		TickComponent->OnTick.Remove(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}
}

void USHGameplayAbility_Interact::AbilityTick(float DeltaTime)
{
	SearchInteractionObjects();
}

void USHGameplayAbility_Interact::SearchInteractionObjects()
{
	if (bIsActive)
	{
		const bool bReplicateEndAbility = true;
		const bool bWasCanceled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}

	ASHCharacter* Character = GetSHCharacterFromActorInfo();
	UWorld* World = GetWorld();
	FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();
	FVector CameraForward = Character->GetCameraComponent()->GetForwardVector();

	if (!IsValid(Character))
	{
		return;
	}

	TSet<AActor*> OverlappingActors;
	Character->GetOverlappingActors(OverlappingActors, ASHInteractableActor::StaticClass());

	TArray<FSHInteractData> ValidActors;
	ValidActors.Reserve(OverlappingActors.Num());

	for (AActor* Actor : OverlappingActors)
	{
		ASHInteractableActor* InteractableActor = Cast<ASHInteractableActor>(Actor);
		if (!IsValid(InteractableActor) || !InteractableActor->GetIsEnabled())
		{
			continue;
		}

		float DistanceSqr = FVector::DistSquared(Character->GetActorLocation(), InteractableActor->GetActorLocation());
		if (DistanceSqr > InteractSearchDistanceSqr)
		{
			continue;
		}

		float Alpha = (FMath::Max(DistanceSqr, InteractDistanceSqr) - InteractDistanceSqr) / (InteractSearchDistanceSqr - InteractDistanceSqr);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(
			FRotationMatrix::MakeFromX(InteractableActor->GetTargetLocation() - CameraLocation).Rotator().Vector().GetSafeNormal(),
			CameraForward.GetSafeNormal()
		)));
		if (Angle > FMath::Lerp(CloseInteractAngle, FarInteractAngle, Alpha))
		{
			continue;
		}

		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(TArray<AActor*>({ Character, InteractableActor }));

		GetWorld()->LineTraceSingleByChannel(HitResult,
			InteractableActor->GetTargetLocation(),
			CameraLocation,
			ECC_Visibility,
			CollisionQueryParams
		);
		if (HitResult.bBlockingHit)
		{
			continue;
		}

		InteractableActor->SetDistanceAlpha(Alpha);
		ValidActors.Add(FSHInteractData(InteractableActor, DistanceSqr));
	}

	if (ValidActors.Num() <= MaxShowWidgets)
	{
		HandleSmallArrayCase(ValidActors);
		return;
	}

	TArray<FSHInteractData> NewClosestActors;
	int32 WorstIndex = INDEX_NONE;
	float CurrentWorstDistSqr = InteractSearchDistanceSqr;

	for(FSHInteractData InteractData : ValidActors)
	{
		if (InteractData.DistanceSqr >= CurrentWorstDistSqr)
		{
			continue;
		}

		if (NewClosestActors.Num() < MaxShowWidgets)
		{
			NewClosestActors.Add(InteractData);

			if (NewClosestActors.Num() == MaxShowWidgets)
			{
				WorstIndex = FindWorstIndex(NewClosestActors);
				CurrentWorstDistSqr = NewClosestActors[FindWorstIndex(NewClosestActors)].DistanceSqr;
			}
		}
		else
		{
			NewClosestActors[WorstIndex] = InteractData;

			WorstIndex = FindWorstIndex(NewClosestActors);
			CurrentWorstDistSqr = NewClosestActors[WorstIndex].DistanceSqr;
		}
	}

	UpdateWidgets(NewClosestActors);
}

void USHGameplayAbility_Interact::HandleSmallArrayCase(const TArray<FSHInteractData>& InArray)
{
	TSet<FSHInteractData> NewSet(InArray);

	for (FSHInteractData PrevInteractData : PreviousClosestActors)
	{
		if (IsValid(PrevInteractData.InteractActor) && !NewSet.Contains(PrevInteractData))
		{
			PrevInteractData.InteractActor->StopCanInteract(GetSHCharacterFromActorInfo());
		}
	}

	for (FSHInteractData NewInteractData : InArray)
	{
		if (IsValid(NewInteractData.InteractActor) && !PreviousClosestActors.Contains(NewInteractData))
		{
			NewInteractData.InteractActor->StartCanInteract(GetSHCharacterFromActorInfo());
		}
	}

	int32 BestIndex = FindBestIndex(InArray);
	if (BestIndex != INDEX_NONE)
	{
		if (PreviousBestIndex == INDEX_NONE)
		{
			InArray[BestIndex].InteractActor->SetShowWidgetKey(true);
		}
		else if (InArray[BestIndex] != PreviousClosestActors[PreviousBestIndex])
		{
			InArray[BestIndex].InteractActor->SetShowWidgetKey(true);
			PreviousClosestActors[PreviousBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}
	else
	{
		if (PreviousBestIndex != INDEX_NONE)
		{
			PreviousClosestActors[PreviousBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}

	PreviousBestIndex = BestIndex;
	PreviousClosestActors = InArray;
}

void USHGameplayAbility_Interact::UpdateWidgets(const TArray<FSHInteractData>& InArray)
{
	TSet<FSHInteractData> NewSet(InArray);
	TSet<FSHInteractData> PrevSet(PreviousClosestActors);

	for (FSHInteractData PrevInteractData : PreviousClosestActors)
	{
		if (IsValid(PrevInteractData.InteractActor) && !NewSet.Contains(PrevInteractData))
		{
			PrevInteractData.InteractActor->StopCanInteract(GetSHCharacterFromActorInfo());
		}
	}

	for (FSHInteractData NewInteractData : InArray)
	{
		if (IsValid(NewInteractData.InteractActor) && !PrevSet.Contains(NewInteractData))
		{
			NewInteractData.InteractActor->StartCanInteract(GetSHCharacterFromActorInfo());
		}
	}

	int32 BestIndex = FindBestIndex(InArray);
	if (BestIndex != INDEX_NONE)
	{
		if (PreviousBestIndex == INDEX_NONE)
		{
			InArray[BestIndex].InteractActor->SetShowWidgetKey(true);
		}
		else if(InArray[BestIndex] != PreviousClosestActors[PreviousBestIndex])
		{
			InArray[BestIndex].InteractActor->SetShowWidgetKey(true);
			PreviousClosestActors[PreviousBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}
	else
	{
		if (PreviousBestIndex != INDEX_NONE)
		{
			PreviousClosestActors[PreviousBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}

	PreviousBestIndex = BestIndex;
	PreviousClosestActors = InArray;
}

int32 USHGameplayAbility_Interact::FindWorstIndex(const TArray<FSHInteractData>& InArray) const
{
	int32 WorstIndex = 0;
	for (int32 i = 1; i < InArray.Num(); i++)
	{
		if (InArray[i].DistanceSqr > InArray[WorstIndex].DistanceSqr)
		{
			WorstIndex = i;
		}
	}

	return WorstIndex;
}

int32 USHGameplayAbility_Interact::FindBestIndex(const TArray<FSHInteractData>& InArray) const
{
	int32 BestIndex = INDEX_NONE;
	int32 ShortestDistanceSqr = InteractDistanceSqr;

	for (int32 i = 0; i < InArray.Num(); i++)
	{
		if (ShortestDistanceSqr > InArray[i].DistanceSqr)
		{
			BestIndex = i;
			ShortestDistanceSqr = InArray[i].DistanceSqr;
		}
	}

	return BestIndex;
}

USHTickManagerComponent* USHGameplayAbility_Interact::FindOrCreateTickManager()
{
	AActor* Owner = GetOwningActorFromActorInfo();

	USHTickManagerComponent* TickManager = Owner->FindComponentByClass<USHTickManagerComponent>();
	if (TickManager != nullptr)
	{
		return TickManager;
	}

	TickManager = NewObject<USHTickManagerComponent>(Owner);
	TickManager->RegisterComponent();

	return TickManager;
}
