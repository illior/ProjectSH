// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SHCoreTypes.h"
#include "SHBlueprintFunctionLibrary.generated.h"

UCLASS()
class PROJECTSH_API USHBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	static FSHActorSaveData SerializeActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	static void UnserializeActor(AActor* InActor, FSHActorSaveData InSaveData);
};
