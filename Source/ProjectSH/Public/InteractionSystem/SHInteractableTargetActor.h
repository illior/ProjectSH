// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem/SHInteractableActor.h"
#include "SHInteractableTargetActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FSHCompleteSignature, ASHInteractableTargetActor, OnComplete, ASHInteractableActor*, ActivatedActor, ASHCharacter*, ActivatedBy);

class UCameraComponent;
class USHItemData;

UCLASS()
class PROJECTSH_API ASHInteractableTargetActor : public ASHInteractableActor
{
	GENERATED_BODY()


public:
	ASHInteractableTargetActor();

	UPROPERTY(BlueprintAssignable, Category = "ProjectSH|InteractionSystem")
	FSHCompleteSignature OnComplete;

	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	virtual void Apply();
	virtual void Cancel();
	virtual void Move(FVector2D Value, float ElapsedTime);
	virtual void MoveWithFrequency(FVector2D Value);
	virtual void AdditiveMove(FVector2D Value, float ElapsedTime);
	virtual void AdditiveMoveWithFrequency(FVector2D Value);

	virtual bool UseItem(USHItemData* InItemData);

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|InteractionSystem")
	virtual void ResumeGame();
	UFUNCTION(BlueprintCallable, Category = "ProjectSH|InteractionSystem")
	virtual void Complete();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|InteractionSystem")
	void ShowItems();
	void ShowItems_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|InteractionSystem")
	void HideItems();
	void HideItems_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ProjectSH|InteractionSystem")
	void SetDescription();
	void SetDescription_Implementation();

	virtual void SetIsEnabled(bool InValue) override;
	virtual void Interact(ASHCharacter* InCharacter) override;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|InteractionSystem")
	TWeakObjectPtr<ASHCharacter> Character;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|InteractionSystem")
	USHInventoryWidget* GetInventoryWidget() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|InteractionSystem")
	void SetItemIsGarbage(USHItemData* InItemData) const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "Apply"))
	void ReceiveApply();
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "Cancel"))
	void ReceiveCancel();

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "Move"))
	void ReceiveMove(FVector2D Value, float ElapsedTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "MoveWithFrequency"))
	void ReceiveMoveWithFrequency(FVector2D Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "AdditiveMove"))
	void ReceiveAdditiveMove(FVector2D Value, float ElapsedTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "AdditiveMoveWithFrequency"))
	void ReceiveAdditiveMoveWithFrequency(FVector2D Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem", meta = (DisplayName = "UseItem"))
	bool ReceiveUseItem(USHItemData* InItemData);
};
