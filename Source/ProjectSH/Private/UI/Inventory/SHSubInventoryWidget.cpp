// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/SHSubInventoryWidget.h"
#include "UI/Inventory/SHInventoryWidget.h"
#include "Components/SHInventoryComponent.h"
#include "Animation/WidgetAnimation.h"

USHInventoryComponent* USHSubInventoryWidget::GetCharacterInventory() const
{
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn == nullptr)
	{
		return nullptr;
	}

	return PlayerPawn->GetComponentByClass<USHInventoryComponent>();
}

void USHSubInventoryWidget::Show(bool bShowAnim)
{
	if (bShowAnim)
	{
		PlayAnimationReverse(DisappearAnim);
		bConsumeAdditiveMove = true;
	}
	else
	{
		SetRenderOpacity(1.0f);
	}
}

void USHSubInventoryWidget::Hide(bool bShowAnim)
{
	if (bShowAnim)
	{
		PlayAnimationForward(DisappearAnim, 2.0f);
		bConsumeAdditiveMove = true;
	}
	else
	{
		SetRenderOpacity(0.0f);
	}
}

void USHSubInventoryWidget::Apply()
{

}

void USHSubInventoryWidget::Cancel()
{
	Close();
}

void USHSubInventoryWidget::Move(FVector2D Value, float ElapsedTime)
{

}

void USHSubInventoryWidget::MoveWithFrequency(FVector2D Value)
{

}

void USHSubInventoryWidget::AdditiveMove(FVector2D Value, float ElapsedTime)
{

}

void USHSubInventoryWidget::AdditiveMoveWithFrequency(FVector2D Value)
{

}

void USHSubInventoryWidget::DisappearAnimationFinished()
{
	if (FMath::IsNearlyZero(GetRenderOpacity()))
	{
		OnDisappearAnimationFinished.Broadcast();
	}

	bConsumeAdditiveMove = false;
}

void USHSubInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent FinishedDelegate;
	FinishedDelegate.BindDynamic(this, &USHSubInventoryWidget::DisappearAnimationFinished);
	BindToAnimationFinished(DisappearAnim, FinishedDelegate);
}

void USHSubInventoryWidget::Close()
{
	USHInventoryWidget* MainWidget = Cast<USHInventoryWidget>(GetOuter());
	if (MainWidget != nullptr)
	{
		MainWidget->Close();
	}
}
