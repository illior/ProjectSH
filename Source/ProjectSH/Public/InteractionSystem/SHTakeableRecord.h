// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem/SHInteractableActor.h"
#include "SHCoreTypes.h"
#include "SHTakeableRecord.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTSH_API ASHTakeableRecord : public ASHInteractableActor
{
	GENERATED_BODY()


public:
	ASHTakeableRecord();

	virtual void Interact(ASHCharacter* InCharacter);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	FDataTableRowHandle RecordRow;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
};
