// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem/SHInteractableActor.h"
#include "SHTakeableItem.generated.h"

class UStaticMeshComponent;
class USHItemData;

UCLASS()
class PROJECTSH_API ASHTakeableItem : public ASHInteractableActor
{
	GENERATED_BODY()


public:
	ASHTakeableItem();

	virtual FSHActorSaveData GetSaveData_Implementation() override;
	virtual void LoadFromSaveData_Implementation(FSHActorSaveData InRecord) override;

	virtual void Interact(ASHCharacter* InCharacter);

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadWrite, Category = "ProjectSH: Interact")
	TObjectPtr<USHItemData> ItemData;

	UPROPERTY(SaveGame)
	TArray<uint8> ItemByteData;

	UFUNCTION()
	void PauseStateChanged(bool bIsPaused);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
};
