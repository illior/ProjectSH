// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SHGameplayAbility.h"
#include "SHGameplayAbility_Interact.generated.h"

class ASHInteractableActor;

USTRUCT()
struct FSHInteractData
{
	GENERATED_BODY()


public:
	FSHInteractData()
	{
		InteractActor = TObjectPtr<ASHInteractableActor>();
		DistanceSqr = 0.0f;
	}

	FSHInteractData(ASHInteractableActor* InActor, float InDistance)
	{
		InteractActor = InActor;
		DistanceSqr = InDistance;
	}

	UPROPERTY()
	float DistanceSqr;

	UPROPERTY()
	TObjectPtr<ASHInteractableActor> InteractActor;

	bool operator==(const FSHInteractData& Other) const
	{
		return InteractActor == Other.InteractActor;
	}

	bool operator!=(const FSHInteractData& other) const
	{
		return !(*this == other);
	}
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FSHInteractData& Thing);
#else
FORCEINLINE uint32 GetTypeHash(const FSHInteractData& Thing)
{
	return GetTypeHash(Thing.InteractActor);
}
#endif

UCLASS()
class PROJECTSH_API USHGameplayAbility_Interact : public USHGameplayAbility
{
	GENERATED_BODY()


public:
	USHGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	float InteractDistance = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH")
	float InteractSearchDistance = 600.0f;

	float InteractDistanceSqr;
	float InteractSearchDistanceSqr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	float CloseInteractAngle = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	float FarInteractAngle = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	int32 MaxShowWidgets = 3;

	UPROPERTY()
	TObjectPtr<class USHTickManagerComponent> TickComponent;

	FDelegateHandle TickDelegateHandle;

	UPROPERTY()
	TArray<FSHInteractData> PreviousClosestActors;
	int32 PreviousBestIndex = INDEX_NONE;

	void AbilityTick(float DeltaTime);

	void SearchInteractionObjects();

	void HandleSmallArrayCase(const TArray<FSHInteractData>& InArray);
	void UpdateWidgets(const TArray<FSHInteractData>& InArray);

	int32 FindWorstIndex(const TArray<FSHInteractData>& InArray) const;
	int32 FindBestIndex(const TArray<FSHInteractData>& InArray) const;

	USHTickManagerComponent* FindOrCreateTickManager();
};
