// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom/InventoryItemButton.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/InventorySlot.h"


UInventoryItemButton::UInventoryItemButton()
{
	OnClicked.AddDynamic(this, &UInventoryItemButton::OnClick);

	//Bind function
	LoadDelegate.AddDynamic(this, &UInventoryItemButton::OnClickItemParts);

	

	// if(PartsModel != nullptr)
	// {
	// 	UE_LOG(LogTemp,Warning,TEXT("GetPawn"));
	// }
}

void UInventoryItemButton::OnClick()
{
	LoadDelegate.Broadcast(this->ItemData, this->InventoryItemType);
}

void UInventoryItemButton::OnClickItemParts(FInventoryItemData Data, EInventoryItemType Type)
{
	PartsModel->ChangeParts(Data, Type);
	
	//UE_LOG(LogTemp,Warning,TEXT("Grid Panel Child Count : %d"), GridPanelRef->GetChildrenCount());

	for (int i= 0 ; i< GridPanelRef->GetChildrenCount(); i++)
	{
		UInventorySlot* ItemSlot = Cast<UInventorySlot>(GridPanelRef->GetChildAt(i));
		FInventoryItemData InventoryItemData = ItemSlot->InventoryItemButton->GetInventoryItemData();
		if(Data.ItemIndex == InventoryItemData.ItemIndex)
		{
			ItemSlot->SetButtonStyle(false);
		}
		else
		{
			ItemSlot->SetButtonStyle(true);
		}
	}
}

void UInventoryItemButton::SettingItemType(FInventoryItemData Data, EInventoryItemType Type)
{
	InventoryItemType = Type;
	ItemData = Data;

	PartsModel = Cast<APartsModel>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	
}

void UInventoryItemButton::SetGridPanel(UGridPanel* GridPanel)
{
	GridPanelRef = GridPanel;
}
