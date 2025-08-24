// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionSystem/SHInteractableTargetActor.h"
#include "Components/SHInteractWidgetComponent.h"
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
	if (IsValid(PlayerController))
	{
		PlayerController->ResumeGame();
	}

	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "SetIsEnabled", true);
	GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);

	StopCanInteract();
}

void ASHInteractableTargetActor::Complete()
{
	if (!Character.IsValid())
	{
		return;
	}

	ASHPlayerController* PlayerController = Cast<ASHPlayerController>(Character->GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->ResumeGame();
	}

	OnComplete.Broadcast(this, Character.Get());
	SetIsEnabled(false);
}

void ASHInteractableTargetActor::ShowItems_Implementation()
{
	if (Character.IsValid())
	{
		USHInventoryWidget* InventoryWidget = GetInventoryWidget();
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->ShowItems();
		}
	}
}

void ASHInteractableTargetActor::HideItems_Implementation()
{
	if (Character.IsValid())
	{
		USHInventoryWidget* InventoryWidget = GetInventoryWidget();
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->HideItems();
			SetDescription();
		}
	}
}

void ASHInteractableTargetActor::SetDescription_Implementation()
{

}

void ASHInteractableTargetActor::SetIsEnabled(bool InValue)
{
	bIsEnabled = InValue;
	if (bIsEnabled)
	{
		GetCollisionComponent()->SetCollisionProfileName(FName(TEXT("OnlyInteract")));

		GetWorldTimerManager().ClearTimer(CooldownTimer);
	}
	else
	{
		GetCollisionComponent()->SetCollisionProfileName(FName(TEXT("NoCollision")));

		GetWidgetComponent()->bShouldShow = false;
		GetWidgetComponent()->bShouldShowKey = false;
		Character.Reset();
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
	GetWidgetComponent()->bShouldShow = false;

	ASHPlayerController* PlayerController = Cast<ASHPlayerController>(InCharacter->GetController());
	if (IsValid(PlayerController))
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 0;
		TransitionParams.BlendExp = 0.0f;
		TransitionParams.BlendFunction = EViewTargetBlendFunction::VTBlend_PreBlended;
		TransitionParams.bLockOutgoing = true;

		PlayerController->SetViewTarget(this, TransitionParams);
	}

	Character = InCharacter;
	OnInteracted.Broadcast(this, InCharacter);
}

USHInventoryWidget* ASHInteractableTargetActor::GetInventoryWidget() const
{
	if (Character.IsValid())
	{
		ASHPlayerController* PlayerController = Character->GetController<ASHPlayerController>();
		if (IsValid(PlayerController))
		{
			ASHGameHUD* HUD = PlayerController->GetHUD<ASHGameHUD>();
			if (IsValid(HUD))
			{
				return HUD->GetInventoryWidget();
			}
		}
	}

	return nullptr;
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
