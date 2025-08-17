// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHInteractItem.generated.h"

class ASHInteractItemTarget;

UCLASS()
class PROJECTSH_API ASHInteractItem : public AActor
{
	GENERATED_BODY()


public:
	ASHInteractItem();

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH")
	TArray<USceneComponent*> GetInteractPoints() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH")
	void ActivateInteractPoint(USceneComponent* TargetComponent);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	ASHInteractItemTarget* GetInteractItemTarget() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void StopInspectItem();
	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void SetConsumeInput(bool InValue);

protected:
	virtual void BeginPlay() override;

};
