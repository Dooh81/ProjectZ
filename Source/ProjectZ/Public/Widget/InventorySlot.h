// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Custom/InventoryItemButton.h"
#include "Data/InventoryItemData.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(Meta=(BindWidget))
	UInventoryItemButton* InventoryItemButton;

	UPROPERTY(Meta=(BindWidget))
	UTextBlock* TextSlot;

	UPROPERTY(meta=(BindWidget))
	UImage* Image_bg;
	
	UPROPERTY(meta=(BindWidget))
	UImage* Image_Equipped;

	FLinearColor DefaultColor = FLinearColor(0.3,0.3,0.3,0.5);

	FLinearColor EquippedColor = FLinearColor(0.3,0.3,0.3,1);
	
	UFUNCTION()
	void GenerateSlot(FInventoryItemData Data, EInventoryItemType Type, bool isOn = true, UGridPanel* GridPanel = nullptr);

	UFUNCTION()
	void SetButtonStyle(bool Enabled);
};
