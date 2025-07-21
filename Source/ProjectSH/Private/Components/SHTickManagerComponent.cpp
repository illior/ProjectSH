// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SHTickManagerComponent.h"

USHTickManagerComponent::USHTickManagerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void USHTickManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OnTick.Broadcast(DeltaTime);
}
