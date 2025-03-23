// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHSubInventoryWidget.generated.h"

class USHInventoryComponent;
class UWidgetAnimation;

DECLARE_MULTICAST_DELEGATE(FOnDisappearAnimationFinishedSignature);

UCLASS()
class PROJECTSH_API USHSubInventoryWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	FOnDisappearAnimationFinishedSignature OnDisappearAnimationFinished;

	bool GetConsumeAdditiveMove() const { return bConsumeAdditiveMove; }

	USHInventoryComponent* GetCharacterInventory() const;

	virtual void Show(bool bShowAnim);
	virtual void Hide(bool bShowAnim);

	virtual void Apply();
	virtual void Cancel();

	virtual void Move(FVector2D Value, float ElapsedTime);
	virtual void MoveWithFrequency(FVector2D Value);
	virtual void AdditiveMove(FVector2D Value, float ElapsedTime);
	virtual void AdditiveMoveWithFrequency(FVector2D Value);

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "ProjectSH")
	UWidgetAnimation* DisappearAnim;

	bool bConsumeAdditiveMove = false;

	UFUNCTION()
	virtual void DisappearAnimationFinished();

	virtual void NativeOnInitialized() override;

	virtual void Close();
};
