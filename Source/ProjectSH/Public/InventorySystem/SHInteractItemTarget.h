// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHCoreTypes.h"
#include "SHInteractItemTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSHItemTargetMoveCompleteSignature);

class USceneCaptureComponent2D;
class UArrowComponent;
class USHItemData;
class ASHInteractItem;

UCLASS()
class PROJECTSH_API ASHInteractItemTarget : public AActor
{
	GENERATED_BODY()


public:
	FSHOnCancelInspectItemSignature OnCancelInspectItem;
	FSHItemTargetMoveCompleteSignature OnMoveComplete;

	ASHInteractItemTarget();

	USceneCaptureComponent2D* GetSceneCaptureComponent() const;
	USceneComponent* GetInteractItemTargetPoint() const;

	void SetConsumeInput(bool InValue);
	bool GetConsumeInput() const;

	virtual void Tick(float DeltaSeconds) override;

	void InputApply();
	void InputCancel();

	void InputMove(FVector2D Value, float ElapsedTime);
	void InputAdditiveMove(FVector2D Value, float ElapsedTime);

	void MoveItemToPosition(FTransform NewTransform, float InTime);

	void StartInspectItem(USHItemData* InItemData, APlayerController* InPlayerController);
	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void StopInspectItem();

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void ActivateInput();

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH")
	void OnStartInspectItem();
	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH")
	void OnStopInspectItem();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	float RotationSpeed = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH")
	float ActivationAngle = 30.0f;

	bool bConsumeInput = false;
	bool bItemMoving = false;

	float TimeToMoveItem = 1.0f;
	float CurrentTimeToMoveItem = 0.0f;

	FTransform StartTransform;
	FTransform TargetTransform;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<USHItemData> InteractItemData;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<ASHInteractItem> InteractItem;
	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TMap<UClass*, ASHInteractItem*> InteractItems;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH")
	TObjectPtr<USceneComponent> InteractItemTargetPoint;

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> FarInspectLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> CloseInspectLocation;
};
