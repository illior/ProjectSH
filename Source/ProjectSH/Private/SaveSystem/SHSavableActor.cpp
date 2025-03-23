// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/SHSavableActor.h"

FSHActorSaveData ISHSavableActor::GetSaveData_Implementation()
{
	/*FSHActorSaveData Record = FSHActorSaveData();

	Record.ActorName = GetFName();
	Record.Transform = GetTransform();

	FMemoryWriter Writer = FMemoryWriter(Record.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);

	return Record;*/

	return FSHActorSaveData();
}

void ISHSavableActor::LoadFromSaveData_Implementation(FSHActorSaveData InRecord)
{
	/*SetActorTransform(InRecord.Transform);

	FMemoryReader Reader = FMemoryReader(InRecord.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);*/
}
