// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetComponents/SHInteractWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "EnhancedInputSubsystems.h"
#include "UI/SHHUD.h"

void USHInteractWidget::StartShow(bool WithKey)
{
	if (IsAnimationPlaying(HideAnim))
	{
		float CurrentAnimTime = GetAnimationCurrentTime(HideAnim);

		StopAllAnimations();
		PlayAnimation(ShowAnim, ShowAnim->GetEndTime() - CurrentAnimTime);

		if (WithKey)
		{
			KeyOverlay->SetRenderOpacity(1.0f);
		}
		else
		{
			KeyOverlay->SetRenderOpacity(0.0f);
		}

		return;
	}

	StopAllAnimations();
	PlayAnimation(ShowAnim);

	if (WithKey)
	{
		KeyOverlay->SetRenderOpacity(1.0f);
	}
	else
	{
		KeyOverlay->SetRenderOpacity(0.0f);
	}
}

void USHInteractWidget::StartHide()
{
	if (IsAnimationPlaying(ShowAnim))
	{
		float CurrentAnimTime = GetAnimationCurrentTime(ShowAnim);

		StopAnimation(ShowAnim);
		PlayAnimation(HideAnim, HideAnim->GetEndTime() - CurrentAnimTime);

		return;
	}

	PlayAnimation(HideAnim);
}

void USHInteractWidget::StartShowKey()
{
	if (IsAnimationPlaying(ShowKeyAnim) || FMath::IsNearlyZero(GetRenderOpacity()))
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
	if (IsAnimationPlaying(HideKeyAnim) || FMath::IsNearlyZero(GetRenderOpacity()))
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

	KeyCodeImage->SetBrushFromSoftTexture(HUD->GetTextureForMappingName(MappingName));
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
