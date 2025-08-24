// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SHInteractWidgetComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSH_API USHInteractWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


public:
	USHInteractWidgetComponent();

	UPROPERTY(BlueprintReadWrite, Category = "ProjectSH")
	bool bShouldShow = false;
	UPROPERTY(BlueprintReadWrite, Category = "ProjectSH")
	bool bShouldShowKey = false;
	UPROPERTY(BlueprintReadWrite, Category = "ProjectSH")
	float DistanceAlpha = 0.0f;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	FVector2D CloseWidgetSize = FVector2D(60, 120);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	FVector2D FarWidgetSize = FVector2D(30, 60);

	bool bShow = false;
	bool bShowKey = false;

	UFUNCTION()
	void WidgetAnimFinished();

	void ShowWidget();
	void ShowWidgetKey();
	void HideWidget();
	void HideWidgetKey();
};
