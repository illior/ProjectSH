// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SHCharacter.h"

#include "Player/SHPlayerController.h"
#include "UI/SHGameHUD.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "UI/Inventory/Items/SHItemsWidget.h"

#include "InteractionSystem/SHInteractableActor.h"
#include "Components/SHInventoryComponent.h"
#include "Components/SHHealthComponent.h"
#include "Components/SHCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ASHCharacter::ASHCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USHCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// SpringArmComponet
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent(), FName(TEXT("head")));
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);

	SpringArmComponent->bUsePawnControlRotation = true;

	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 40.0f;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 30.0f;

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// Inventory
	InventoryComponent = CreateDefaultSubobject<USHInventoryComponent>(TEXT("CharacterInventory"));

	// Health
	HealthComponent = CreateDefaultSubobject<USHHealthComponent>(TEXT("CharacterHealth"));
}

FVector ASHCharacter::GetCameraLocation() const
{
	if (CameraComponent != nullptr)
	{
		return CameraComponent->GetComponentLocation();
	}

	return GetActorLocation();
}

void ASHCharacter::AddSlots(int32 Count)
{
	ASHPlayerController* PlayerController = GetController<ASHPlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->OpenItems();
	InventoryComponent->AddSlots(Count);
}

void ASHCharacter::AddItem(USHItemData* InItemData)
{
	ASHPlayerController* PlayerController = GetController<ASHPlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	ASHGameHUD* HUD = PlayerController->GetHUD<ASHGameHUD>();
	if (HUD == nullptr)
	{
		return;
	}

	USHInventoryWidget* InventoryWidget = HUD->GetInventoryWidget();
	if (InventoryWidget == nullptr)
	{
		return;
	}

	USHItemsWidget* ItemsWidget = InventoryWidget->GetItemsWidget();
	if (ItemsWidget == nullptr)
	{
		return;
	}

	PlayerController->OpenItems();
	ItemsWidget->TryAddItem(InItemData);
}

void ASHCharacter::AddRecord(FSHRecord InRecord)
{
	ASHPlayerController* PlayerController = GetController<ASHPlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->OpenRecords();
	InventoryComponent->AddRecord(InRecord);
}

void ASHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchInteractableActor();
}

void ASHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASHCharacter::InputLook);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ASHCharacter::InputStartMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASHCharacter::InputMove);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASHCharacter::InputStartInteract);
	}
}

void ASHCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController != nullptr)
	{
		APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
		if (PlayerCameraManager != nullptr)
		{
			PlayerCameraManager->ViewPitchMax = CameraPitchBound.Y;
			PlayerCameraManager->ViewPitchMin = CameraPitchBound.X;
		}
	}
}

void ASHCharacter::SearchInteractableActor()
{
	FVector StartPosition = GetCameraLocation();
	FVector EndPosition = StartPosition + CameraComponent->GetForwardVector() * InteractSearchDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPosition,
		EndPosition,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(InteractCapsuleRadius),
		Params
	);

	if (!HitResult.bBlockingHit)
	{
		if (InteractActor.IsValid())
		{
			InteractActor->StopCanInteract(this);
			InteractActor.Reset();
		}

		return;
	}

	ASHInteractableActor* FindedActor = Cast<ASHInteractableActor>(HitResult.GetActor());
	if (FindedActor == nullptr || !FindedActor->GetIsEnabled())
	{
		if (InteractActor.IsValid())
		{
			InteractActor->StopCanInteract(this);
			InteractActor.Reset();
		}

		return;
	}

	if (FindedActor != InteractActor.Get())
	{
		if (InteractActor.IsValid())
		{
			InteractActor->StopCanInteract(this);
		}

		InteractActor = FindedActor;
		InteractActor->StartCanInteract(this, FVector::Dist(StartPosition, InteractActor->GetTargetLocation()));
	}
}

void ASHCharacter::InputLook(const FInputActionInstance& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	FVector2D LookAxisVector = Value.GetValue().Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASHCharacter::InputStartMove(const FInputActionInstance& Value)
{

}

void ASHCharacter::InputMove(const FInputActionInstance& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	FVector2D MovementVector = Value.GetValue().Get<FVector2D>();
	const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASHCharacter::InputStartInteract(const FInputActionInstance& Value)
{
	if (!InteractActor.IsValid())
	{
		return;
	}

	float Distance = FVector::Dist(InteractActor->GetTargetLocation(), GetCameraLocation());
	if (Distance <= InteractDistance)
	{
		InteractActor->Interact(this);
		InteractActor.Reset();
	}
}
