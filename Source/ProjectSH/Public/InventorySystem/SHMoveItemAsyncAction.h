// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SHMoveItemAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSHMoveItemCompletedSignature);

class ASHInteractItemTarget;

UCLASS()
class PROJECTSH_API USHMoveItemAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable)
	FSHMoveItemCompletedSignature OnCompleted;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static USHMoveItemAsyncAction* MoveItemToTarget(UObject* WorldContextObject, ASHInteractItemTarget* ItemTarget, FTransform Position, float Time);

	virtual void Activate() override;

protected:
	UFUNCTION()
	void Complete();

	ASHInteractItemTarget* ItemTarget;
	FTransform Position;
	float Time;
};
