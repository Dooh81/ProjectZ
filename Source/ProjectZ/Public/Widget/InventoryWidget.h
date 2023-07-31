// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Commons/Commons.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Custom/PartsModel.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="InventorySlot")
	TSubclassOf<UUserWidget>  InventorySlotClass;

	UPROPERTY(Meta=(BindWidget))
	UGridPanel* GridPanel;

	UPROPERTY(Meta=(BindWidget))
	UButton* Button_Main;
	
	//Server All Inventory Data
	TArray<int32> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PartsModel")
	APartsModel* PartsModel;

	/*
	 * All Inventory data classification Part Items
	 */
	TArray<int32> FrameItems;  //1
	TArray<int32> WheelItems; //2
	TArray<int32> HeadgearItems; //3
	TArray<int32> GlassesItems; //4
	TArray<int32> JerseyItems; //5
	TArray<int32> GlovesItems; //6
	TArray<int32> SocksItems; //7
	TArray<int32> ShoesItems; //8

	TArray<int32> SelectItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="DataTable")
	class UDataTable* InventoryTable;
	
	UFUNCTION(BlueprintCallable, Category = "Parts")
	void CallPartsItems(EInventoryItemType ItemType);

	UFUNCTION(BlueprintCallable, Category = "Parts")
	void AllDisablePartsModel();

	UFUNCTION(BlueprintCallable)
	void ShowHighlight(EInventoryItemType ItemType);

	UFUNCTION()
	void OnClickedMainMenu();
	
};