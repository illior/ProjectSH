// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SHSavableActor.h"
#include "SHInteractableActor.generated.h"

class ASHCharacter;
class UCapsuleComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FSHInteractSignature, ASHInteractableActor, OnInteracted, ASHInteractableActor*, ActivatedActor, ASHCharacter*, ActivatedBy);

DECLARE_LOG_CATEGORY_EXTERN(LogInteractableActor, Log, All);

UCLASS()
class PROJECTSH_API ASHInteractableActor : public AActor, public ISHSavableActor
{
	GENERATED_BODY()
	

public:	
	ASHInteractableActor();

	UPROPERTY(BlueprintAssignable, Category = "ProjectSH|InteractionSystem")
	FSHInteractSignature OnInteracted;

	UCapsuleComponent* GetCollision() const { return CollisionComponent; }
	UWidgetComponent* GetWidget() const { return WidgetComponent; }

	virtual FSHActorSaveData GetSaveData_Implementation() override;
	virtual void LoadFromSaveData_Implementation(FSHActorSaveData InRecord) override;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH|InteractionSystem")
	virtual void SetIsEnabled(bool InValue);
	bool GetIsEnabled() const { return bIsEnabled; };

	virtual void StartCanInteract(ASHCharacter* InCharacter, float InDistance);
	virtual void StopCanInteract(ASHCharacter* InCharacter);

	virtual void Interact(ASHCharacter* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category = "ProjectSH|InteractionSystem")
	void IsEnabledChanged(bool IsEnabled);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "ProjectSH: Interact")
	bool bIsEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	bool bIsReusable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact", meta = (EditCondition = "bIsReusable"))
	float CooldownTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	FVector2D CloseWidgetSize = FVector2D(100, 200);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Interact")
	FVector2D FarWidgetSize = FVector2D(50, 100);

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|InteractionSystem")
	TWeakObjectPtr<ASHCharacter> Character;

	FTimerHandle CooldownTimer;
	FTimerHandle CheckDistanceTimer;
	bool bShowKey;

	UFUNCTION()
	void WidgetAnimFinished();

	virtual void BeginPlay() override;

	void CheckDistance();

	void ShowWidget();
	void ShowWidgetKey();
	void HideWidget();
	void HideWidgetKey();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
