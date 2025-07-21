// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHInteractableActor.h"
#include "Player/SHCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SHInteractWidgetComponent.h"

DEFINE_LOG_CATEGORY(LogInteractableActor);

ASHInteractableActor::ASHInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsEnabled = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	CollisionComponent->SetupAttachment(GetRootComponent());
	CollisionComponent->SetCollisionProfileName(FName(TEXT("OnlyInteract")));
	CollisionComponent->SetVisibility(false);

	CollisionComponent->SetCapsuleHalfHeight(600.0f);
	CollisionComponent->SetCapsuleRadius(600.0f);

	WidgetComponent = CreateDefaultSubobject<USHInteractWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

FSHActorSaveData ASHInteractableActor::GetSaveData_Implementation()
{
	FSHActorSaveData Record = FSHActorSaveData();

	Record.ActorName = GetFName();
	Record.Transform = GetTransform();

	if (!bIsEnabled && GetWorldTimerManager().IsTimerActive(CooldownTimer))
	{
		bIsEnabled = true;
	}

	FMemoryWriter Writer = FMemoryWriter(Record.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Writer, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);

	if (bIsEnabled && GetWorldTimerManager().IsTimerActive(CooldownTimer))
	{
		bIsEnabled = false;
	}

	return Record;
}

void ASHInteractableActor::LoadFromSaveData_Implementation(FSHActorSaveData InRecord)
{
	SetActorTransform(InRecord.Transform);

	FMemoryReader Reader = FMemoryReader(InRecord.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(Reader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;

	Serialize(Ar);
}

FVector ASHInteractableActor::GetTargetLocation(AActor* RequestedBy) const
{
	return WidgetComponent->GetComponentLocation();
}

void ASHInteractableActor::SetIsEnabled(bool InValue)
{
	UE_LOG(LogInteractableActor, Display, TEXT("%s: SetIsEnabled %s"), *GetName(), InValue ? TEXT("True") : TEXT("False"));

	bIsEnabled = InValue;
	if (bIsEnabled)
	{
		CollisionComponent->SetCollisionProfileName(FName(TEXT("OnlyInteract")));

		GetWorldTimerManager().ClearTimer(CooldownTimer);
	}
	else
	{
		CollisionComponent->SetCollisionProfileName(FName(TEXT("NoCollision")));

		WidgetComponent->bShouldShow = false;
		WidgetComponent->bShouldShowKey = false;
		Character.Reset();
	}

	IsEnabledChanged(bIsEnabled);
}

void ASHInteractableActor::StartCanInteract(ASHCharacter* InCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("StartCanInteract %s"), *GetName());

	if (InCharacter == nullptr)
	{
		return;
	}

	Character = InCharacter;

	WidgetComponent->bShouldShow = true;
}

void ASHInteractableActor::StopCanInteract(ASHCharacter* InCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("StopCanInteract %s"), *GetName());

	WidgetComponent->bShouldShow = false;
	WidgetComponent->bShouldShowKey = false;

	Character.Reset();
}

void ASHInteractableActor::Interact(ASHCharacter* InCharacter)
{
	OnInteracted.Broadcast(this, InCharacter);

	bIsEnabled = false;
	StopCanInteract(Character.Get());

	if (bIsReusable)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "SetIsEnabled", true);

		GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
	}
}

void ASHInteractableActor::SetShowWidgetKey(bool InValue)
{
	WidgetComponent->bShouldShowKey = InValue;
}

void ASHInteractableActor::SetDistanceAlpha(float InValue)
{
	WidgetComponent->DistanceAlpha = InValue;
}

void ASHInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsEnabled)
	{
		SetIsEnabled(false);
	}
}
