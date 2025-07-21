// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SHCoreTypes.h"
#include "SHCharacter.generated.h"

class USHAbilitySystemComponent;

class USpringArmComponent;
class UCameraComponent;
class USHInventoryComponent;
class USHHealthComponent;

class USHItemData;

class UInputAction;
struct FInputActionInstance;

UCLASS()
class PROJECTSH_API ASHCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	ASHCharacter(const FObjectInitializer& ObjInit);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCameraComponent* GetCameraComponent() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void AddSlots(int32 Count = 2);

	void AddItem(USHItemData* InItemData);

	void AddRecord(FSHRecord InRecord);

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH: Camera")
	FVector2D CameraPitchBound = FVector2D(-75.0f, 75.0f);

	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|AbilitySystem")
	TObjectPtr<USHAbilitySystemComponent> AbilitySystemComponent;

	virtual void BeginPlay() override;
private:
	void InitAbilitySystemComponet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USHInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USHHealthComponent> HealthComponent;

	void InputLook(const FInputActionInstance& Value);
	void InputMove(const FInputActionInstance& Value);
};
