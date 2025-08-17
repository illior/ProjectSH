// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SHInteractItemTarget.h"
#include "InventorySystem/SHMoveItemAsyncAction.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ArrowComponent.h"
#include "InventorySystem/SHInteractItem.h"
#include "InventorySystem/SHItemData.h"

ASHInteractItemTarget::ASHInteractItemTarget()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	FarInspectLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("FarArrow"));
	FarInspectLocation->SetupAttachment(GetRootComponent());

	CloseInspectLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CloseArrow"));
	CloseInspectLocation->SetupAttachment(GetRootComponent());

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCaptureComponent->SetupAttachment(GetRootComponent());
	SceneCaptureComponent->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent->SetTickableWhenPaused(true);
}

USceneCaptureComponent2D* ASHInteractItemTarget::GetSceneCaptureComponent() const
{
	return SceneCaptureComponent;
}

USceneComponent* ASHInteractItemTarget::GetInteractItemTargetPoint() const
{
	return InteractItemTargetPoint;
}

void ASHInteractItemTarget::SetConsumeInput(bool InValue)
{
	bConsumeInput = InValue;
}

bool ASHInteractItemTarget::GetConsumeInput() const
{
	return bConsumeInput || bItemMoving;
}

void ASHInteractItemTarget::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InteractItem)
	{
		InteractItemTargetPoint = nullptr;
		TArray<USceneComponent*> InteractPoints = InteractItem->GetInteractPoints();
		if (InteractPoints.Num() != 0)
		{
			float AngleToTargetPoint = ActivationAngle;
			for (USceneComponent* TargetPoint : InteractPoints)
			{
				float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct((-1.0f * SceneCaptureComponent->GetForwardVector()).GetSafeNormal(), TargetPoint->GetForwardVector().GetSafeNormal())));

				if (Angle < AngleToTargetPoint)
				{
					InteractItemTargetPoint = TargetPoint;
				}
			}
		}

		if (bItemMoving)
		{
			CurrentTimeToMoveItem += DeltaSeconds;
			float Alpha = FMath::Min(1.0f, CurrentTimeToMoveItem / TimeToMoveItem);

			FVector NewPosition = FMath::Lerp(StartTransform.GetLocation(), TargetTransform.GetLocation(), Alpha);
			FRotator NewRotation = FQuat::Slerp(StartTransform.GetRotation(), TargetTransform.GetRotation(), Alpha).Rotator();

			InteractItem->SetActorLocationAndRotation(NewPosition, NewRotation);
			if (Alpha == 1.0f)
			{
				bItemMoving = false;
				OnMoveComplete.Broadcast();
			}
		}
	}
}

void ASHInteractItemTarget::InputApply()
{
	if (InteractItem && InteractItemTargetPoint)
	{
		InteractItem->ActivateInteractPoint(InteractItemTargetPoint);
	}
}

void ASHInteractItemTarget::InputCancel()
{
	StopInspectItem();
}

void ASHInteractItemTarget::InputMove(FVector2D Value, float ElapsedTime)
{
	if (IsValid(InteractItem))
	{
		FRotator DeltaRotation;
		DeltaRotation.Pitch = Value.Y * -RotationSpeed * GetWorld()->DeltaRealTimeSeconds;
		DeltaRotation.Yaw = Value.X * -RotationSpeed * GetWorld()->DeltaRealTimeSeconds;

		InteractItem->AddActorWorldRotation(DeltaRotation);
	}
}

void ASHInteractItemTarget::InputAdditiveMove(FVector2D Value, float ElapsedTime)
{
	if (IsValid(InteractItem))
	{
		FRotator DeltaRotation;
		DeltaRotation.Roll = Value.X * RotationSpeed * GetWorld()->DeltaRealTimeSeconds;

		InteractItem->AddActorWorldRotation(DeltaRotation);
	}
}

void ASHInteractItemTarget::MoveItemToPosition(FTransform NewTransform, float InTime)
{
	if (!InteractItem)
	{
		return;
	}

	StartTransform = InteractItem->GetActorTransform();
	TargetTransform = NewTransform;

	TimeToMoveItem = InTime;
	CurrentTimeToMoveItem = 0.0f;

	bItemMoving = true;
}

void ASHInteractItemTarget::StartInspectItem(USHItemData* InItemData, APlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
	InteractItemData = InItemData;
	InItemData->GetInteractItemClass().Get();

	UClass* ItemClass = InItemData->GetInteractItemClass().LoadSynchronous();
	if (ItemClass == nullptr)
	{
		StopInspectItem();
		return;
	}

	InteractItem = InteractItems.FindRef(ItemClass);
	if (!IsValid(InteractItem))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		InteractItem = GetWorld()->SpawnActor<ASHInteractItem>(ItemClass, FTransform(GetActorLocation()), SpawnParameters);
		InteractItems.Add(ItemClass, InteractItem);

		SceneCaptureComponent->ShowOnlyActorComponents(InteractItem);
	}

	InteractItem->SetActorTransform(FarInspectLocation->GetComponentTransform());
	InteractItem->SetActorHiddenInGame(false);

	bConsumeInput = false;
	OnStartInspectItem();
}

void ASHInteractItemTarget::StopInspectItem()
{
	if (IsValid(InteractItem))
	{
		InteractItem->DisableInput(PlayerController);
		InteractItem->SetActorHiddenInGame(true);
		InteractItem = nullptr;
	}

	InteractItemData = nullptr;
	OnStopInspectItem();
	OnCancelInspectItem.Broadcast();
}

void ASHInteractItemTarget::ActivateInput()
{
	if (!InteractItem || !PlayerController)
	{
		return;
	}

	InteractItem->EnableInput(PlayerController);
	bConsumeInput = true;
}

void ASHInteractItemTarget::BeginPlay()
{
	Super::BeginPlay();
	
}
