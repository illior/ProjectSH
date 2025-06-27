// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHInteractableTargetActor.h"
#include "UI/SHGameHUD.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "UI/Inventory/Items/SHItemsWidget.h"
#include "Player/SHPlayerController.h"
#include "Player/SHCharacter.h"
#include "InventorySystem/SHItemData.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

ASHInteractableTargetActor::ASHInteractableTargetActor()
{
	SetTickableWhenPaused(true);
	CooldownTime = 0.5f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetTickableWhenPaused(true);
	CameraComponent->PostProcessSettings.bOverride_MotionBlurMax = true;
	CameraComponent->PostProcessSettings.MotionBlurMax = 0.0f;
}

void ASHInteractableTargetActor::Apply()
{
	ReceiveApply();
}

void ASHInteractableTargetActor::Cancel()
{
	ReceiveCancel();
}

void ASHInteractableTargetActor::Move(FVector2D Value, float ElapsedTime)
{
	ReceiveMove(Value, ElapsedTime);
}

void ASHInteractableTargetActor::MoveWithFrequency(FVector2D Value)
{
	ReceiveMoveWithFrequency(Value);
}

void ASHInteractableTargetActor::AdditiveMove(FVector2D Value, float ElapsedTime)
{
	ReceiveAdditiveMove(Value, ElapsedTime);
}

void ASHInteractableTargetActor::AdditiveMoveWithFrequency(FVector2D Value)
{
	ReceiveAdditiveMoveWithFrequency(Value);
}

bool ASHInteractableTargetActor::UseItem(USHItemData* InItemData)
{
	return ReceiveUseItem(InItemData);
}

void ASHInteractableTargetActor::ResumeGame()
{
	if (!Character.IsValid())
	{
		return;
	}

	ASHPlayerController* PlayerController = Cast<ASHPlayerController>(Character->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ResumeGame();
	Character.Reset();

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "SetIsEnabled", true);

	GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
}

void ASHInteractableTargetActor::Complete()
{
	if (!Character.IsValid())
	{
		return;
	}

	ASHPlayerController* PlayerController = Cast<ASHPlayerController>(Character->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ResumeGame();
	OnComplete.Broadcast(this, Character.Get());

	SetIsEnabled(false);

	UE_LOG(LogTemp, Warning, TEXT("Complete"));
}

void ASHInteractableTargetActor::ShowItems_Implementation()
{
	if (!Character.IsValid())
	{
		return;
	}

	USHInventoryWidget* InventoryWidget = GetInventoryWidget();
	if (!IsValid(InventoryWidget))
	{
		return;
	}

	InventoryWidget->ShowItems();
}

void ASHInteractableTargetActor::HideItems_Implementation()
{
	if (!Character.IsValid())
	{
		return;
	}

	USHInventoryWidget* InventoryWidget = GetInventoryWidget();
	if (!IsValid(InventoryWidget))
	{
		return;
	}

	InventoryWidget->HideItems();
	SetDescription();
}

void ASHInteractableTargetActor::SetDescription_Implementation()
{

}

void ASHInteractableTargetActor::SetIsEnabled(bool InValue)
{
	bIsEnabled = InValue;
	if (bIsEnabled)
	{
		GetCollision()->SetCollisionProfileName(FName(TEXT("OnlyInteract")));

		GetWorldTimerManager().ClearTimer(CooldownTimer);
	}
	else
	{
		GetCollision()->SetCollisionProfileName(FName(TEXT("NoCollision")));

		GetWidget()->SetVisibility(false);
		Character.Reset();

		GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
	}

	IsEnabledChanged(bIsEnabled);
}

void ASHInteractableTargetActor::Interact(ASHCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}

	bIsEnabled = false;
	HideWidget();

	ASHPlayerController* PlayerController = Cast<ASHPlayerController>(InCharacter->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 0;
	TransitionParams.BlendExp = 0.0f;
	TransitionParams.BlendFunction = EViewTargetBlendFunction::VTBlend_PreBlended;
	TransitionParams.bLockOutgoing = true;

	PlayerController->SetViewTarget(this, TransitionParams);

	OnInteracted.Broadcast(this, InCharacter);
}

USHInventoryWidget* ASHInteractableTargetActor::GetInventoryWidget() const
{
	if (!Character.IsValid())
	{
		return nullptr;
	}

	ASHPlayerController* PlayerController = Character->GetController<ASHPlayerController>();
	if (PlayerController == nullptr)
	{
		return nullptr;
	}

	ASHGameHUD* HUD = PlayerController->GetHUD<ASHGameHUD>();
	if (HUD == nullptr)
	{
		return nullptr;
	}


	return HUD->GetInventoryWidget();
}

void ASHInteractableTargetActor::SetItemIsGarbage(USHItemData* InItemData) const
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([InItemData]
	{
		if (IsValid(InItemData))
		{
			InItemData->MarkAsGarbage();
		}
	});

	GetWorldTimerManager().SetTimerForNextTick(TimerDelegate);
}
