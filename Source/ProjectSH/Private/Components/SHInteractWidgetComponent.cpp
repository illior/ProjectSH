// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SHInteractWidgetComponent.h"
#include "UI/WidgetComponents/SHInteractWidget.h"

USHInteractWidgetComponent::USHInteractWidgetComponent()
{
	SetTickGroup(ETickingGroup::TG_PostUpdateWork);

	SetWidgetSpace(EWidgetSpace::Screen);
	SetVisibleFlag(false);
	SetDrawAtDesiredSize(false);
	SetPivot(FVector2D(0.5f, 1.0f));
	SetDrawSize(FarWidgetSize);

	ConstructorHelpers::FClassFinder<USHInteractWidget> WidgetBPClass(TEXT("/Game/Game/UI/WidgetComponents/WBP_Interact"));
	if (WidgetBPClass.Class != nullptr)
	{
		SetWidgetClass(WidgetBPClass.Class);
	}
}

void USHInteractWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(GetUserWidgetObject());
	if (InteractWidget != nullptr)
	{
		FWidgetAnimationDynamicEvent FinishedDelegate;
		FinishedDelegate.BindDynamic(this, &USHInteractWidgetComponent::WidgetAnimFinished);

		InteractWidget->BindToAnimationFinished(InteractWidget->GetHideAnim(), FinishedDelegate);
	}
}

void USHInteractWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldShow)
	{
		if (!bShow)
		{
			ShowWidget();
		}

		if (bShouldShowKey)
		{
			if (!bShowKey)
			{
				ShowWidgetKey();
			}
		}
		else
		{
			if (bShowKey)
			{
				HideWidgetKey();
			}
		}
	}
	else
	{
		if (bShow)
		{
			HideWidget();
		}
	}

	if (bShow)
	{
		SetDrawSize(FMath::Lerp(CloseWidgetSize, FarWidgetSize, DistanceAlpha));
	}
}

void USHInteractWidgetComponent::WidgetAnimFinished()
{
	if (!bShow)
	{
		SetVisibility(false);
	}
}

void USHInteractWidgetComponent::ShowWidget()
{
	bShow = true;
	SetVisibility(bShow);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]
	{
		USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(GetUserWidgetObject());
		if (IsValid(InteractWidget))
		{
			InteractWidget->StartShow(bShowKey);
		}
	});
}

void USHInteractWidgetComponent::ShowWidgetKey()
{
	bShowKey = true;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(GetUserWidgetObject());
	if (IsValid(InteractWidget))
	{
		InteractWidget->StartShowKey();
	}
}

void USHInteractWidgetComponent::HideWidget()
{
	bShow = false;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(GetUserWidgetObject());
	if (IsValid(InteractWidget))
	{
		InteractWidget->StartHide();
	}
}

void USHInteractWidgetComponent::HideWidgetKey()
{
	bShowKey = false;

	USHInteractWidget* InteractWidget = Cast<USHInteractWidget>(GetUserWidgetObject());
	if (IsValid(InteractWidget))
	{
		InteractWidget->StartHideKey();
	}
}
