// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHTakeableRecord.h"
#include "Components/SHInteractWidgetComponent.h"
#include "Player/SHCharacter.h"

ASHTakeableRecord::ASHTakeableRecord()
{
	bIsReusable = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionProfileName(FName("NoCollision"));
}

void ASHTakeableRecord::Interact(ASHCharacter* InCharacter)
{
	OnInteracted.Broadcast(this, InCharacter);

	bIsEnabled = false;
	GetWidgetComponent()->bShouldShow = false;

	FSHRecord* Record = RecordRow.GetRow<FSHRecord>("");

	if (Record != nullptr)
	{
		InCharacter->AddRecord(*Record);
	}

	SetLifeSpan(GetWorld()->DeltaTimeSeconds);
}
