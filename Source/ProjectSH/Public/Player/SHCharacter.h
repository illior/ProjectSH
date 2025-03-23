// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SHCoreTypes.h"
#include "SHCharacter.generated.h"

class ASHInteractableActor;
class USpringArmComponent;
class UCameraComponent;
class USHInventoryComponent;
class USHHealthComponent;

class USHItemData;

class UInputAction;
struct FInputActionInstance;

UCLASS()
class PROJECTSH_API ASHCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	ASHCharacter(const FObjectInitializer& ObjInit);

	UInputAction* GetLookAction() const { return LookAction; };
	UInputAction* GetMoveAction() const { return MoveAction; };
	UInputAction* GetCrouchAction() const { return CrouchAction; };
	UInputAction* GetSprintAction() const { return SprintAction; };
	UInputAction* GetInteractAction() const { return InteractAction; };

	float GetInteractDistance() const { return InteractDistance; };
	float GetInteractSearchDistance() const { return InteractSearchDistance; };
	FVector GetCameraLocation() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectSH")
	void AddSlots(int32 Count = 2);

	void AddItem(USHItemData* InItemData);

	void AddRecord(FSHRecord InRecord);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectSH: Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH: Camera")
	FVector2D CameraPitchBound = FVector2D(-75.0f, 75.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH: Interact")
	float InteractDistance = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH: Interact")
	float InteractSearchDistance = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectSH: Interact")
	float InteractCapsuleRadius = 20.0f;

	UPROPERTY()
	TWeakObjectPtr<ASHInteractableActor> InteractActor;

	virtual void BeginPlay() override;

	void SearchInteractableActor();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USHInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USHHealthComponent> HealthComponent;

	void InputLook(const FInputActionInstance& Value);

	void InputStartMove(const FInputActionInstance& Value);
	void InputMove(const FInputActionInstance& Value);

	void InputStartInteract(const FInputActionInstance& Value);
};
