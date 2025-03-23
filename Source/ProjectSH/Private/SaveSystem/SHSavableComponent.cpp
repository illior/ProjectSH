// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/SHSavableComponent.h"

FSHComponentSaveData ISHSavableComponent::GetSaveData_Implementation()
{
	return FSHComponentSaveData();
}

void ISHSavableComponent::LoadFromSaveData_Implementation(FSHComponentSaveData InRecord)
{

}
