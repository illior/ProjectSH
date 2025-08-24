// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystem/SHInteractableActor.h"
#include "SHDoorActor.generated.h"

class UStaticMeshComponent;
class UArrowComponent;

UENUM(BlueprintType)
enum class ESHDoorState : uint8
{
	Close,
	SlightlyOpen,
	Open
};

UENUM(BlueprintType)
enum class ESHDoorDirection : uint8
{
	Forward,
	Backward
};

UCLASS()
class PROJECTSH_API ASHDoorActor : public ASHInteractableActor
{
	GENERATED_BODY()


public:
	ASHDoorActor();

	UStaticMeshComponent* GetDoorFrameComponent() const;
	UStaticMeshComponent* GetDoorComponent() const;
	UArrowComponent* GetArrowComponent() const;

	void SlightlyOpenDoor();
	void OpenDoor();
	void CloseDoor();

	virtual void Interact(ASHCharacter* InCharacter) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "ProjectSH|InteractionSystem")
	TWeakObjectPtr<ASHCharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	FVector ForwardOpenLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	FVector BackwardOpenLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "ProjectSH: Door")
	bool bIsDoorLocked = false;

	bool bMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	float TimeToSlightlyOpen = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	float TimeToOpen = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	float TimeToClose = 0.5f;

	float ElapsedTime = 0.0f;
	float TargetTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	float OpenAngle = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Door")
	float SlightlyOpenAngle = 25.0f;

	float InitialAngle = 0.0f;
	float TargetAngle = 0.0f;

	ESHDoorState CurrentState = ESHDoorState::Close;
	ESHDoorState NextState = ESHDoorState::Close;

	ESHDoorDirection Direction = ESHDoorDirection::Forward;

	void RotateDoorAroundPoint(float Angle, FVector PointToRotateAround, FVector AxisToRotateAround);
	float GetAngleRelativeDoor(AActor* OtherActor);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorFrameComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DoorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ArrowComponent;
};
