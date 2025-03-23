// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetComponents/SHInteractWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "EnhancedInputSubsystems.h"
#include "Player/SHCharacter.h"
#include "UI/SHHUD.h"

void USHInteractWidget::StartShow(bool WithKey)
{
	StopAllAnimations();

	PlayAnimation(ShowAnim);

	if (WithKey)
	{
		KeyCodeImage->SetRenderOpacity(1.0f);
	}
	else
	{
		KeyCodeImage->SetRenderOpacity(0.0f);
	}
}

void USHInteractWidget::StartHide()
{
	if (IsAnimationPlaying(ShowAnim))
	{
		float CurrentAnimTime = GetAnimationCurrentTime(ShowAnim);

		StopAllAnimations();
		PlayAnimation(HideAnim, HideAnim->GetEndTime() - CurrentAnimTime);
	}

	StopAllAnimations();
	PlayAnimation(HideAnim);
}

void USHInteractWidget::StartShowKey()
{
	if (IsAnimationPlaying(HideAnim) || IsAnimationPlaying(ShowKeyAnim) || FMath::IsNearlyZero(GetRenderOpacity()))
	{
		return;
	}

	if (IsAnimationPlaying(HideKeyAnim))
	{
		float CurrentAnimTime = GetAnimationCurrentTime(HideKeyAnim);

		StopAnimation(HideKeyAnim);
		PlayAnimation(ShowKeyAnim, ShowKeyAnim->GetEndTime() - CurrentAnimTime);
	}

	PlayAnimation(ShowKeyAnim);
}

void USHInteractWidget::StartHideKey()
{
	if (IsAnimationPlaying(HideAnim) || IsAnimationPlaying(HideKeyAnim) || FMath::IsNearlyZero(GetRenderOpacity()))
	{
		return;
	}

	if (IsAnimationPlaying(ShowKeyAnim))
	{
		float CurrentAnimTime = GetAnimationCurrentTime(ShowKeyAnim);

		StopAnimation(ShowKeyAnim);
		PlayAnimation(HideKeyAnim, HideKeyAnim->GetEndTime() - CurrentAnimTime);
	}

	PlayAnimation(HideKeyAnim);
}

void USHInteractWidget::MappingsRebuilt()
{
	ASHHUD* HUD = IsValid(GetOwningPlayer()) ? GetOwningPlayer()->GetHUD<ASHHUD>() : nullptr;
	if (!IsValid(HUD))
	{
		return;
	}

	ASHCharacter* Character = GetOwningPlayerPawn<ASHCharacter>();
	if (!IsValid(Character))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(Character->GetInteractAction());
	if (Keys.IsValidIndex(0))
	{
		KeyCodeImage->SetBrushFromSoftTexture(HUD->GetTextureForKey(Keys[0]));
	}
}

void USHInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	Subsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &USHInteractWidget::MappingsRebuilt);
}
