// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SHBlueprintFunctionLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

FSHActorSaveData USHBlueprintFunctionLibrary::SerializeActor(AActor* InActor)
{
	FSHActorSaveData Record = FSHActorSaveData();

	Record.ActorName = InActor->GetFName();
	Record.Transform = InActor->GetTransform();

	FMemoryWriter Writer = FMemoryWriter(Record.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	InActor->Serialize(Ar);

	return Record;
}

void USHBlueprintFunctionLibrary::UnserializeActor(AActor* InActor, FSHActorSaveData InSaveData)
{
	InActor->SetActorTransform(InSaveData.Transform);

	FMemoryReader Reader = FMemoryReader(InSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	InActor->Serialize(Ar);
}
