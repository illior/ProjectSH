// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHDoorActor.h"
#include "Player/SHCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SHInteractWidgetComponent.h"

ASHDoorActor::ASHDoorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	DoorFrameComponent->SetupAttachment(GetRootComponent());
	DoorFrameComponent->SetCollisionProfileName(FName(TEXT("BlockAll")));

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	DoorComponent->SetupAttachment(GetRootComponent());
	DoorComponent->SetCollisionProfileName(FName(TEXT("BlockAllDynamic")));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComponent->SetupAttachment(GetRootComponent());

	GetWidgetComponent()->SetupAttachment(DoorComponent, FName("Handle"));
}

UStaticMeshComponent* ASHDoorActor::GetDoorFrameComponent() const
{
	return DoorFrameComponent;
}

UStaticMeshComponent* ASHDoorActor::GetDoorComponent() const
{
	return DoorComponent;
}

UArrowComponent* ASHDoorActor::GetArrowComponent() const
{
	return ArrowComponent;
}

void ASHDoorActor::SlightlyOpenDoor()
{
	if (NextState == ESHDoorState::SlightlyOpen)
	{
		if (ElapsedTime > TimeToSlightlyOpen / 2)
		{
			OpenDoor();
		}
		return;
	}

	bMove = true;

	InitialAngle = DoorComponent->GetRelativeRotation().Yaw;
	TargetAngle = SlightlyOpenAngle * (Direction == ESHDoorDirection::Forward ? -1.0f : 1.0f);

	ElapsedTime = 0.0f;
	TargetTime = TimeToSlightlyOpen;

	NextState = ESHDoorState::SlightlyOpen;
}

void ASHDoorActor::OpenDoor()
{
	bMove = true;

	InitialAngle = DoorComponent->GetRelativeRotation().Yaw;
	TargetAngle = OpenAngle * (Direction == ESHDoorDirection::Forward ? -1.0f : 1.0f);

	ElapsedTime = 0.0f;
	TargetTime = TimeToOpen;

	NextState = ESHDoorState::Open;
	
	bIsEnabled = false;

	StopCanInteract();
}

void ASHDoorActor::CloseDoor()
{
	bMove = true;

	InitialAngle = DoorComponent->GetRelativeRotation().Yaw;
	TargetAngle = 0.0f;

	ElapsedTime = 0.0f;
	TargetTime = TimeToClose;

	NextState = ESHDoorState::Close;
}

void ASHDoorActor::Interact(ASHCharacter* InCharacter)
{
	if (!bIsDoorLocked)
	{
		float AngleBetweenDoorAndCharacter;
		switch (CurrentState)
		{
		case ESHDoorState::Close:
			AngleBetweenDoorAndCharacter = GetAngleRelativeDoor(InCharacter);
			Direction = AngleBetweenDoorAndCharacter < 90.0f ? ESHDoorDirection::Forward : ESHDoorDirection::Backward;

			Character = InCharacter;
			SlightlyOpenDoor();
			break;
		case ESHDoorState::SlightlyOpen:
			Character = InCharacter;
			OpenDoor();
			break;
		case ESHDoorState::Open:
			CloseDoor();
			break;
		}
	}

	OnInteracted.Broadcast(this, InCharacter);
}

void ASHDoorActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMove)
	{
		ElapsedTime = FMath::Min(TargetTime, ElapsedTime + DeltaSeconds);

		RotateDoorAroundPoint(
			FMath::Lerp(InitialAngle, TargetAngle, ElapsedTime / TargetTime) - DoorComponent->GetRelativeRotation().Yaw,
			Direction == ESHDoorDirection::Forward ? ForwardOpenLocation : BackwardOpenLocation,
			FVector(0.0f, 0.0f, 1.0f)
		);

		if (ElapsedTime == TargetTime)
		{
			bMove = false;
			CurrentState = NextState;
		}
	}
}

void ASHDoorActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor == Character)
	{
		CloseDoor();
		bIsEnabled = true;
	}
}

void ASHDoorActor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other->IsA<ASHCharacter>() && MyComp == DoorComponent)
	{
		if (bIsEnabled && !bIsDoorLocked && CurrentState != ESHDoorState::Open)
		{
			float AngleBetweenDoorAndCharacter = GetAngleRelativeDoor(Other);
			Direction = AngleBetweenDoorAndCharacter < 90.0f ? ESHDoorDirection::Forward : ESHDoorDirection::Backward;

			Character = Cast<ASHCharacter>(Other);
			SlightlyOpenDoor();
		}
	}
}

void ASHDoorActor::RotateDoorAroundPoint(float Angle, FVector PointToRotateAround, FVector AxisToRotateAround)
{
	FVector NewLocation = (DoorComponent->GetRelativeLocation() - PointToRotateAround).RotateAngleAxis(Angle, AxisToRotateAround.GetSafeNormal()) + PointToRotateAround;
	FRotator NewRotation = FRotator(FQuat(DoorComponent->GetRelativeRotation()) * FQuat(AxisToRotateAround.GetSafeNormal(), FMath::DegreesToRadians(Angle)));

	DoorComponent->SetRelativeLocationAndRotation(NewLocation, NewRotation);
}

float ASHDoorActor::GetAngleRelativeDoor(AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return 0.0f;
	}

	return FMath::RadiansToDegrees(
		FMath::Acos(
			FVector::DotProduct(
				FRotationMatrix::MakeFromX(ArrowComponent->GetComponentLocation() - OtherActor->GetActorLocation()).Rotator().Vector().GetSafeNormal(),
				ArrowComponent->GetForwardVector().GetSafeNormal()
		))
	);
}
