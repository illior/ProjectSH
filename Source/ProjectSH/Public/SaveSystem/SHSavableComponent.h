// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SHCoreTypes.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SHSavableComponent.generated.h"

UINTERFACE(MinimalAPI)
class USHSavableComponent : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSH_API ISHSavableComponent
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|SaveGame")
	FSHComponentSaveData GetSaveData();
	virtual FSHComponentSaveData GetSaveData_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|SaveGame")
	void LoadFromSaveData(FSHComponentSaveData InRecord);
	virtual void LoadFromSaveData_Implementation(FSHComponentSaveData InRecord);
};
