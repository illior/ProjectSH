// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHCoreTypes.h"
#include "SHSlotsContainerWidget.generated.h"

class UUniformGridPanel;
class USHSlotWidget;

UCLASS()
class PROJECTSH_API USHSlotsContainerWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	FIntPoint GetInventorySize() const;
	int32 GetCurrentSlotsCount() const;

	void SetSlotPadding(FMargin InPadding);
	FMargin GetSlotPadding() const;

	ESHSlotState GetSlotState(FIntPoint InPosition) const;

	bool CanClose() const;

	void SetSlotSelected(FIntPoint InPosition);
	void SetSlotUnselected(FIntPoint InPosition);

	void CreateSlots();
	void UpdateSlots();
	void AddSlots(int32 InCount);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectSH")
	TObjectPtr<UUniformGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	TSubclassOf<USHSlotWidget> SlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSH: Child Layout")
	FMargin SlotPadding = FMargin();

	UPROPERTY()
	TArray<TObjectPtr<USHSlotWidget>> Slots;

	FIntPoint InventorySize;
	int32 CurrentSlotsCount;

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
};
