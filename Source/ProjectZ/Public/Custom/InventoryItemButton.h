// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartsModel.h"
#include "Components/Button.h"
#include "Commons/Commons.h"
#include "Components/GridPanel.h"
#include "Data/InventoryItemData.h"
#include "InventoryItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLoadDelegate,FInventoryItemData, Data, EInventoryItemType, ItemType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClickeDelegate);

/**
 * 
 */
UCLASS()
class PROJECTZ_API UInventoryItemButton : public UButton
{
	GENERATED_BODY()

public:
	UInventoryItemButton();

	UPROPERTY()
	FLoadDelegate LoadDelegate;

	UPROPERTY()
	FClickeDelegate ClickeDelegate;

	UFUNCTION()
	void OnClick();
	
	UFUNCTION()
	void OnClickItemParts(FInventoryItemData Data, EInventoryItemType Type);

	UFUNCTION(BlueprintCallable, Category = "InventoryItemSetting")
	void SettingItemType(FInventoryItemData Data, EInventoryItemType Type);

	UFUNCTION()
	void SetGridPanel(UGridPanel* GridPanel);

	UFUNCTION()
	FInventoryItemData GetInventoryItemData();

private:
	UPROPERTY()
	EInventoryItemType InventoryItemType;

	UPROPERTY()
	FInventoryItemData ItemData;

	UPROPERTY()
	APartsModel* PartsModel;

	UPROPERTY()
	UGridPanel* GridPanelRef;
};

inline FInventoryItemData UInventoryItemButton::GetInventoryItemData()
{
	return ItemData;
}
