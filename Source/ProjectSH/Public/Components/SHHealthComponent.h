// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSH_API USHHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	

public:
	USHHealthComponent();

protected:
	virtual void BeginPlay() override;

};
