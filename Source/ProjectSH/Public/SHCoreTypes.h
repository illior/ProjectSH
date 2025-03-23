#pragma once

#include "CoreMinimal.h"
#include "SHCoreTypes.generated.h"

class USHBaseItem;

UENUM(BlueprintType)
enum class ESHSlotState : uint8 {
	Void	UMETA(DisplayName = "Void"),
	Item	UMETA(DisplayName = "Item"),
	Block	UMETA(DisplayName = "Block"),
};

UENUM(BlueprintType)
enum class ESHAmmoType : uint8
{
	Pistol	UMETA(DisplayName = "Pistol"),
	Shotgun	UMETA(DisplayName = "Shotgun")
};

USTRUCT(BlueprintType)
struct FSHCraftRecipe
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USHBaseItem> FirstItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USHBaseItem> SecondItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USHBaseItem> Result;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 Count;
};

USTRUCT(BlueprintType)
struct FSHRecord : public FTableRowBase
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	TArray<FText> Pages;
};

UENUM(BlueprintType)
enum class ESHButtonState : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Hovered		UMETA(DisplayName = "Hover"),
	Pressed		UMETA(DisplayName = "Pressed"),
	Disabled	UMETA(DisplayName = "Disable")
};

UENUM()
enum class ESHItemInventoryState : uint8 {
	Select,
	DropDownMenu,
	MovingItem,
	InspectItem,
	BindWeapon
};

UENUM()
enum class ESHInventoryOpenState : uint8 {
	Default,
	AddSlots,
	AddItem,
	Interact
};

USTRUCT(BlueprintType)
struct FSHActorSaveData
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT(BlueprintType)
struct FSHComponentSaveData
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FName ComponentName;

	UPROPERTY()
	FTransform RelativeTransform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT(BlueprintType)
struct FSHPlayerSaveData
{
	GENERATED_BODY()


public:
	UPROPERTY()
	FVector Position;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FRotator ControlRotation;

	UPROPERTY()
	TArray<uint8> CharacterByteData;

	UPROPERTY()
	TArray<uint8> PlayerStateByteData;

	UPROPERTY()
	TMap<FName, FSHComponentSaveData> ComponentsData;
};