// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHInspectItemWidget.generated.h"

class ASHInteractItemTarget;
class USHItemData;

UCLASS()
class PROJECTSH_API USHInspectItemWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	FSHOnCancelInspectItemSignature OnCancelInspectItem;

	FVector2D WorldToScreenPosition(const FVector& WorldLocation);

	void StartInspect(USHItemData* InItemData);
	void StopInspect();

	void InputApply();
	void InputCancel();

	void InputMove(FVector2D Value, float ElapsedTime);
	void InputAdditiveMove(FVector2D Value, float ElapsedTime);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<class USHInteractWidget> InteractWidget;

	UPROPERTY(EditAnywhere, Category = "ProjectSH")
	TSubclassOf<ASHInteractItemTarget> InteractItemTargetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASHInteractItemTarget> InteractItemTarget;

	bool bShowWidget;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	ASHInteractItemTarget* FindOrCreateTarget();
};
