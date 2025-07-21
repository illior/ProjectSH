// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHTickManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSHTickDelegate, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSH_API USHTickManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USHTickManagerComponent();

	FSHTickDelegate OnTick;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
