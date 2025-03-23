// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SHCoreTypes.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SHSavableActor.generated.h"

UINTERFACE(MinimalAPI)
class USHSavableActor : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSH_API ISHSavableActor
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|SaveGame")
	FSHActorSaveData GetSaveData();
	virtual FSHActorSaveData GetSaveData_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|SaveGame")
	void LoadFromSaveData(FSHActorSaveData InRecord);
	virtual void LoadFromSaveData_Implementation(FSHActorSaveData InRecord);
};
