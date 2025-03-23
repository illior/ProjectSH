// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHInteractableActor.h"
#include "Player/SHCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/WidgetComponents/SHInteractWidget.h"

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

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	WidgetComponent->SetDrawSize(FarWidgetSize);
	WidgetComponent->SetDrawAtDesiredSize(false);
	WidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));

	WidgetComponent->SetVisibility(false);

	ConstructorHelpers::FClassFinder<USHInteractWidget> WidgetBPClass(TEXT("/Game/Game/UI/WidgetComponents/WBP_Interact"));
	if (WidgetBPClass.Class != nullptr)
	{
		WidgetComponent->SetWidgetClass(WidgetBPClass.Class);
	}
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

		HideWidget();
		Character.Reset();

		GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
	}

	IsEnabledChanged(bIsEnabled);
}

void ASHInteractableActor::StartCanInteract(ASHCharacter* InCharacter, float InDistance)
{
	UE_LOG(LogInteractableActor, Display, TEXT("%s: StartCanInteract"), *GetName());

	if (InCharacter == nullptr)
	{
		return;
	}

	Character = InCharacter;

	bShowKey = InDistance <= InCharacter->GetInteractDistance();
	ShowWidget();

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ASHInteractableActor::CheckDistance);
	GetWorldTimerManager().SetTimer(CheckDistanceTimer, Delegate, 0.001f, FTimerManagerTimerParameters{.bLoop = true, .bMaxOncePerFrame = true});
}

void ASHInteractableActor::StopCanInteract(ASHCharacter* InCharacter)
{
	UE_LOG(LogInteractableActor, Display, TEXT("%s: StopCanInteract"), *GetName());

	HideWidget();
	Character.Reset();

	GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
}

void ASHInteractableActor::Interact(ASHCharacter* InCharacter)
{
	OnInteracted.Broadcast(this, InCharacter);

	bIsEnabled = false;
	Character.Reset();
	HideWidget();

	if (bIsReusable)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "SetIsEnabled", true);

		GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
	}
}

void ASHInteractableActor::WidgetAnimFinished()
{
	if (!Character.IsValid())
	{
		WidgetComponent->SetVisibility(false);
	}
}

void ASHInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		FWidgetAnimationDynamicEvent FinishedDelegate;
		FinishedDelegate.BindDynamic(this, &ASHInteractableActor::WidgetAnimFinished);

		InteractWidget->BindToAnimationFinished(InteractWidget->GetHideAnim(), FinishedDelegate);
	}

	if (!bIsEnabled)
	{
		SetIsEnabled(false);
	}
}

void ASHInteractableActor::CheckDistance()
{
	if (Character.IsValid())
	{
		float Distance = FVector::Dist(Character->GetCameraLocation(), GetTargetLocation());
		if (Distance > Character->GetInteractDistance())
		{
			if (bShowKey)
			{
				HideWidgetKey();
			}
		}
		else
		{
			if (!bShowKey)
			{
				ShowWidgetKey();
			}
		}

		float Alpha = (FMath::Max(Distance, Character->GetInteractDistance()) - Character->GetInteractDistance())
			/ (Character->GetInteractSearchDistance() - Character->GetInteractDistance());

		WidgetComponent->SetDrawSize(FMath::Lerp(CloseWidgetSize, FarWidgetSize, Alpha));
	}
}

void ASHInteractableActor::ShowWidget()
{
	WidgetComponent->SetVisibility(true);

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		InteractWidget->StartShow(bShowKey);
	}
}

void ASHInteractableActor::ShowWidgetKey()
{
	bShowKey = true;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		InteractWidget->StartShowKey();
	}
}

void ASHInteractableActor::HideWidget()
{
	bShowKey = false;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		InteractWidget->StartHide();
	}
}

void ASHInteractableActor::HideWidgetKey()
{
	bShowKey = false;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(WidgetComponent->GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		InteractWidget->StartHideKey();
	}
}
