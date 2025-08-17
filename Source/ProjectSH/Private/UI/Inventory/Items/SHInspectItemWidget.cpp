// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Items/SHInspectItemWidget.h"
#include "UI/WidgetComponents/SHInteractWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

#include "InventorySystem/SHInteractItemTarget.h"
#include "InventorySystem/SHItemData.h"
#include "EngineUtils.h"

FVector2D USHInspectItemWidget::WorldToScreenPosition(const FVector& WorldLocation)
{
	USceneCaptureComponent2D* CaptureComponent = InteractItemTarget->GetSceneCaptureComponent();

	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;

	FMinimalViewInfo MinimalViewInfo;
	CaptureComponent->GetCameraView(GetWorld()->DeltaRealTimeSeconds, MinimalViewInfo);
	UGameplayStatics::GetViewProjectionMatrix(MinimalViewInfo, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);

	FVector4 WorldPoint = FVector4(WorldLocation, 1.0f);
	FVector4 ViewPoint = ViewMatrix.TransformFVector4(WorldPoint);

	FVector4 ProjectionPoint = ProjectionMatrix.TransformFVector4(ViewPoint);

	if (ProjectionPoint.W != 0.0f)
	{
		ProjectionPoint.X /= ProjectionPoint.W;
		ProjectionPoint.Y /= ProjectionPoint.W;
		ProjectionPoint.Z /= ProjectionPoint.W;
	}

	FVector2D UV;
	UV.X = (ProjectionPoint.X + 1.0f) / 2.0f;
	UV.Y = 1.0f - (ProjectionPoint.Y + 1.0f) / 2.0f;

	UImage* RenderTargetImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("RenderTarget"))));

	FGeometry Geometry = RenderTargetImage->GetCachedGeometry();
	FVector2D WidgetSize = Geometry.GetLocalSize();

	float RenderTargetWidth = CaptureComponent->TextureTarget->SizeX;
	float RenderTargetHeight = CaptureComponent->TextureTarget->SizeY;

	FVector2D WidgetPosition;
	WidgetPosition.X = UV.X * WidgetSize.X * (RenderTargetWidth / WidgetSize.X);
	WidgetPosition.Y = UV.Y * WidgetSize.Y * (RenderTargetHeight / WidgetSize.Y);

	return WidgetPosition;
}

void USHInspectItemWidget::StartInspect(USHItemData* InItemData)
{
	if (InItemData->GetInteractItemClass().IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartInspect call with InteractItemClass nullptr"));

		OnCancelInspectItem.Broadcast();
		return;
	}

	if (!IsValid(InteractItemTarget))
	{
		InteractItemTarget = FindOrCreateTarget();
	}

	SetVisibility(ESlateVisibility::Visible);
	InteractItemTarget->StartInspectItem(InItemData, GetOwningPlayer());
}

void USHInspectItemWidget::StopInspect()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnCancelInspectItem.Broadcast();
}

void USHInspectItemWidget::InputApply()
{
	if (IsValid(InteractItemTarget) && !InteractItemTarget->GetConsumeInput())
	{
		InteractItemTarget->InputApply();
	}
}

void USHInspectItemWidget::InputCancel()
{
	if (IsValid(InteractItemTarget) && !InteractItemTarget->GetConsumeInput())
	{
		InteractItemTarget->InputCancel();
	}
}

void USHInspectItemWidget::InputMove(FVector2D Value, float ElapsedTime)
{
	if (IsValid(InteractItemTarget) && !InteractItemTarget->GetConsumeInput())
	{
		InteractItemTarget->InputMove(Value, ElapsedTime);
	}
}

void USHInspectItemWidget::InputAdditiveMove(FVector2D Value, float ElapsedTime)
{
	if (IsValid(InteractItemTarget) && !InteractItemTarget->GetConsumeInput())
	{
		InteractItemTarget->InputAdditiveMove(Value, ElapsedTime);
	}
}

void USHInspectItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(InteractItemTarget) || !IsValid(InteractItemTarget->GetInteractItemTargetPoint()) || InteractItemTarget->GetConsumeInput())
	{
		if (bShowWidget)
		{
			bShowWidget = false;
			InteractWidget->StartHide();
		}
		return;
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(InteractWidget->Slot);
	if (IsValid(CanvasSlot))
	{
		CanvasSlot->SetPosition(WorldToScreenPosition(InteractItemTarget->GetInteractItemTargetPoint()->GetComponentLocation()));
	}

	if (!bShowWidget)
	{
		bShowWidget = true;
		InteractWidget->StartShow(false);
	}
}

ASHInteractItemTarget* USHInspectItemWidget::FindOrCreateTarget()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	for (TActorIterator<AActor> It(World, InteractItemTargetClass); It; ++It)
	{
		ASHInteractItemTarget* Actor = Cast<ASHInteractItemTarget>(*It);
		if (IsValid(Actor))
		{
			Actor->OnCancelInspectItem.AddUObject(this, &USHInspectItemWidget::StopInspect);

			return Actor;
		}
	}

	ASHInteractItemTarget* Actor = World->SpawnActor<ASHInteractItemTarget>(InteractItemTargetClass, FTransform(FVector(0.0f, 0.0f, -1000.0f)));
	Actor->OnCancelInspectItem.AddUObject(this, &USHInspectItemWidget::StopInspect);
	return Actor;
}
